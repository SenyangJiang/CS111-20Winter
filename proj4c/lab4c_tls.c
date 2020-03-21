// NAME: Senyang Jiang
// EMAIL: senyangjiang@yahoo.com
// ID: 505111806

#include <mraa.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <getopt.h>
#include <math.h>
#include <poll.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/ssl.h>

const int B = 4275;
const int R0 = 100000;

int log_flag = 0;
int id_flag = 0;
int host_flag = 0;
int port_flag = 0;
int report_flag = 1;

char id[10];
int portno;
int period = 1;
char scale = 'F';
char* hostname = NULL;
char* logname = NULL;
FILE* logfile;
int sockfd;
SSL* ssl_client;

mraa_aio_context temp;

void free_and_exit(int num) {
  free(logname);
  free(hostname);
  exit(num);
}

void client_shutdown() {
  char time_report[32];
  char report[64];
  struct tm* timestamp;
  time_t t;
  t = time(NULL);
  timestamp = localtime(&t);
  
  strftime(time_report, 32, "%H:%M:%S", timestamp);
  sprintf(report, "%s SHUTDOWN\n", time_report);
  SSL_write(ssl_client, report, strlen(report));
  fprintf(logfile, "%s SHUTDOWN\n", time_report);
  
  fclose(logfile);
  mraa_aio_close(temp);
  if(SSL_shutdown(ssl_client) < 0) {
    fprintf(stderr, "fail to shutdown SSL\n");
    free_and_exit(2);
  }
  SSL_free(ssl_client);
  free_and_exit(0);
}

void command_interpret(char* command) {
  char* period_command = strstr(command, "PERIOD=");

  fprintf(logfile, command);
  
  int len = strlen(command);
  command[len-1] = '\0'; // change '\n' to '\0' (easier to interpret)
  if(strcmp(command, "SCALE=F") == 0) {
    scale = 'F';
  }
  else if(strcmp(command, "SCALE=C") == 0) {
    scale = 'C';
  }
  else if(period_command != NULL) {
    period_command += 7;
    if(strlen(period_command) == 0) {
      return;
    }
    int i;
    for(i = 0; period_command[i] != '\0'; i++) {
      if(!isdigit(period_command[i]))
	return;
    }
    period = atoi(period_command);
  }
  else if(strcmp(command, "STOP") == 0) {
    report_flag = 0;
  }
  else if(strcmp(command, "START") == 0) {
    report_flag = 1;
  }
  else if(strcmp(command, "OFF") == 0) {
    client_shutdown();
  }

}

int main(int argc, char **argv)
{ 
  int opt;
  struct option long_options[] = {{"period", required_argument, NULL, 'p'},
				  {"scale", required_argument, NULL, 's'},
				  {"log", required_argument, NULL, 'l'},
				  {"id", required_argument, NULL, 'i'},
				  {"host", required_argument, NULL, 'h'},
				  {0, 0, 0, 0}};
  while(optind < argc)
    {
      if((opt = getopt_long(argc, argv, "+:", long_options, NULL)) != -1) {
	switch(opt)
	  {
	  case 'p':
	    period = atoi(optarg);
	    break;
	  case 's':
	    if(strlen(optarg) != 1) {
	      fprintf(stderr, "invalid temperature scale\n");
	      free_and_exit(1);
	    }
	    scale = optarg[0];
	    if(scale != 'C' && scale != 'F') {
	      fprintf(stderr, "invalid temperature scale\n");
	      free_and_exit(1);
	    }
	    break;
	  case 'l':
	    log_flag = 1;
	    logname = malloc(sizeof(char)*(strlen(optarg)+1));
	    strcpy(logname, optarg);
	    break;
	  case 'i':
	    id_flag = 1;
	    if(strlen(optarg) != 9) {
	      fprintf(stderr, "id has to be 9 digits\n");
	      free_and_exit(1);
	    }
	    int i;
	    for(i = 0; optarg[i] != '\0'; i++) {
	      if(!isdigit(optarg[i])) {
		fprintf(stderr, "id is not a number\n");
		free_and_exit(1);
	      }
	    }
	    strcpy(id, optarg);
	    break;
	  case 'h':
	    host_flag = 1;
	    hostname = malloc(sizeof(char)*(strlen(optarg)+1));
	    strcpy(hostname, optarg);
	    break;
	  case '?':
	    fprintf(stderr, "Unrecognized argument: %s\n", argv[optind-1]);
	    free_and_exit(1);
	    break;
	  case ':':
	    fprintf(stderr, "Missing argument for %s\n", argv[optind-1]);
	    free_and_exit(1);
	    break;
	  }
      }
      else { // handles non-switch parameter
	if(port_flag) {
	  fprintf(stderr, "too many non-switch arguments\n");
	  free_and_exit(1);
	}
	port_flag = 1;
        portno = atoi(argv[optind]);
	optind++;
      }
    }
  // check mandatory arguments
  if(!id_flag) {
    fprintf(stderr, "Missing mandatory argument --id\n");
    free_and_exit(1);
  }

  if(!host_flag) {
    fprintf(stderr, "Missing mandatory argument --host\n");
    free_and_exit(1);
  }
  
  if(!log_flag) {
    fprintf(stderr, "Missing mandatory argument --log\n");
    free_and_exit(1);
  }
  
  if(!port_flag) {
    fprintf(stderr, "Missing mandatory port number\n");
    free_and_exit(1);
  }
  
  // create logfile
  logfile = fopen(logname, "w");
  if(logfile == NULL) {
    fprintf(stderr, "fail to create logfile\n");
    free_and_exit(2);
  }
  
  // initialize IO for temperature sensor
  temp = mraa_aio_init(1);
  if(temp == NULL) {
    fprintf(stderr, "Failed to initialize temperature sensor\n");
    mraa_deinit();
    free_and_exit(2);
  }

  // SSL setup and initialization
  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();
  SSL_CTX* ctx = SSL_CTX_new(TLSv1_client_method());
  if(ctx == NULL) {
    fprintf(stderr, "fail to get SSL context object\n");
    free_and_exit(2);
  }
  ssl_client = SSL_new(ctx);
  if(ssl_client == NULL) {
    fprintf(stderr, "fail to create SSL client\n");
    free_and_exit(2);
  }
  
  // open a TCP connection to the server
  struct sockaddr_in serv_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    fprintf(stderr, "error opening socket\n");
    free_and_exit(2);
  }

  server = gethostbyname(hostname);
  if(server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    free_and_exit(1);
  }
  bzero((char*) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char*) server->h_addr,
	(char*)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "error connecting to server\n");
    free_and_exit(2);
  }

  // SSL connection
  if(SSL_set_fd(ssl_client, sockfd) == 0) {
    fprintf(stderr, "fail to associate the created TCP socket with the SSL client\n");
    free_and_exit(2);
  }
  if(SSL_connect(ssl_client) < 1) {
    fprintf(stderr, "fail to establish a SSL connection\n");
    free_and_exit(2);
  }
  
  // immediately send (and log) an ID terminated with a newline
  char report[64];
  sprintf(report, "ID=%s\n", id);
  SSL_write(ssl_client, report, strlen(report));
  fprintf(logfile, "ID=%s\n", id);

  // send (and log) newline terminated temperature reports over the connection
  struct tm* timestamp;
  time_t curr_sec;
  time_t last_sec = 0;;
  char time_report[32];
  char buffer[256];
  char partial_line[64];
  struct pollfd pfd;
  pfd.fd = sockfd;
  pfd.events = POLLIN;
  int ret;
  int n;
  int index = 0;
  
  while(1) {
    curr_sec = time(NULL);
    if(last_sec == 0 || curr_sec - last_sec >= period) {
      // generate report if report_flag is 1
      last_sec = curr_sec;
      int a = mraa_aio_read(temp);
      float R = 1023.0/a-1.0;
      R = R0*R;

      float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature(Celsius)
      if(scale == 'F')
	temperature = temperature * 1.8 + 32;

      if(report_flag) {
      	timestamp = localtime(&curr_sec);
      	strftime(time_report, 32, "%H:%M:%S", timestamp);

      	sprintf(report, "%s %.1f\n", time_report, temperature);
	SSL_write(ssl_client, report, strlen(report));
	fprintf(logfile, "%s %.1f\n", time_report, temperature);
      }
    }
    
    // accept commands from stdin
    ret = poll(&pfd, 1, 0);
    if(ret > 0) {
      n = SSL_read(ssl_client, buffer, 256);
      int i;
      for(i = 0; i < n; i++) {
	partial_line[index] = buffer[i];
	if(buffer[i] == '\n') {
	  partial_line[index+1] = '\0';
	  command_interpret(partial_line);
	  index = 0;
	}
	else {
	  index++;
	}
      }
    } 
  }
}
