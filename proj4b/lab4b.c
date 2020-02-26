#include <mraa.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <getopt.h>
#include <math.h>
#include <poll.h>

const int B = 4275;
const int R0 = 100000;

int period = 1;
char scale = 'F';
int log_flag = 0;
char* logname = NULL;
int report_flag = 1;
FILE* logfile;

mraa_aio_context temp;
mraa_gpio_context button;

void shutdown() {
  char time_report[32];
  struct tm* timestamp;
  time_t t;
  t = time(NULL);
  timestamp = localtime(&t);
  strftime(time_report, 32, "%H:%M:%S", timestamp);
  fprintf(stdout, "%s SHUTDOWN\n", time_report);
  if(log_flag) {
    fprintf(logfile, "%s SHUTDOWN\n", time_report);
  }
  mraa_aio_close(temp);
  mraa_gpio_close(button);
  exit(0);
}

void command_interpret(const char* command) {
  char* period_command = strstr(command, "PERIOD=");
  char* log_command = strstr(command, "LOG");

  if(log_flag) {
    fprintf(logfile, command);
  }
  
  if(strcmp(command, "SCALE=F\n") == 0) {
    scale = 'F';
  }
  else if(strcmp(command, "SCALE=C\n") == 0) {
    scale = 'C';
  }
  else if(period_command != NULL) {
    period_command += 7;
    period = atoi(period_command);
  }
  else if(strcmp(command, "STOP\n") == 0) {
    report_flag = 0;
  }
  else if(strcmp(command, "START\n") == 0) {
    report_flag = 1;
  }
  else if(log_command != NULL) {
    ;
  }
  else if(strcmp(command, "OFF\n") == 0) {
    shutdown();
  }

}

int main(int argc, char **argv)
{ 
  int opt;
  struct option long_options[] = {{"period", required_argument, NULL, 'p'},
				  {"scale", required_argument, NULL, 's'},
				  {"log", required_argument, NULL, 'l'},
				  {0, 0, 0, 0}};
  while(1)
    {
      opt = getopt_long(argc, argv, "+:", long_options, NULL);
      if(opt == -1)
	break;

      switch(opt)
	{
	case 'p':
	  period = atoi(optarg);
	  break;
	case 's':
	  if(strlen(optarg) > 1) {
	    fprintf(stderr, "invalid temperature scale\n");
	    exit(1);
	  }
	  scale = optarg[0];
	  if(scale != 'C' && scale != 'F') {
	    fprintf(stderr, "invalid temperature scale\n");
	    exit(1);
	  }
	  break;
	case 'l':
	  log_flag = 1;
	  logname = malloc(sizeof(char)*(strlen(optarg)+1));
	  strcpy(logname, optarg);
	  break;
	case '?':
	  fprintf(stderr, "Unrecognized argument: %s\n", argv[optind-1]);
	  free(logname);
	  exit(1);
	  break;
	case ':':
	  fprintf(stderr, "Missing argument for %s\n", argv[optind-1]);
	  free(logname);
	  exit(1);
	  break;
	}
    }

  // check if there is an element that is not an option
  if(optind != argc)
    {
      fprintf(stderr, "Unrecognized argument: %s\n", argv[optind]);
      free(logname);
      exit(1);
    }

  // create logfile if specified
  if(log_flag) {
    logfile = fopen(logname, "w");
    if(logfile == NULL) {
      fprintf(stderr, "fail to create logfile\n");
      exit(1);
    }
  }
  
  // initialize IO for temperature sensor
  temp = mraa_aio_init(1);
  if(temp == NULL) {
    fprintf(stderr, "Failed to initialize temperature sensor\n");
    mraa_deinit();
    exit(1);
  }

  // initialize IO for button and arrange for an interrupt when the button is pushed
  button = mraa_gpio_init(60);
  if(button == NULL) {
    fprintf(stderr, "Failed to initialize button\n");
    mraa_deinit();
    exit(1);
  }
  mraa_gpio_dir(button, MRAA_GPIO_IN);
  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &shutdown, NULL);


  struct tm* timestamp;
  time_t curr_sec;
  time_t last_sec = 0;;
  char time_report[32];
  char buffer[256];
  char partial_line[64];
  struct pollfd pfd;
  pfd.fd = STDIN_FILENO;
  pfd.events = POLLIN;
  int ret;
  int n;
  int index = 0;
  // main loop
  while(1) {
    curr_sec = time(NULL);
    if(report_flag && (last_sec == 0 || curr_sec - last_sec >= period)) {
      // generate report
      last_sec = curr_sec;
      int a = mraa_aio_read(temp);
      float R = 1023.0/a-1.0;
      R = R0*R;

      float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature(Celsius)
      if(scale == 'F')
	temperature = temperature * 1.8 + 32;

      timestamp = localtime(&curr_sec);
      strftime(time_report, 32, "%H:%M:%S", timestamp);
    
      fprintf(stdout, "%s %.1f\n", time_report, temperature);
      if(log_flag) {
	fprintf(logfile, "%s %.1f\n", time_report, temperature);
      }
    }
    
    // accept commands from stdin
    ret = poll(&pfd, 1, 0);
    if(ret > 0) {
      n = read(pfd.fd, buffer, 256);
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
