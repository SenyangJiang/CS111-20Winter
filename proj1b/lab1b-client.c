// NAME: Senyang Jiang
// EMAIL: senyangjiang@yahoo.com
// ID: 505111806

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ulimit.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "zlib.h"

#define BUFSIZE 2048
struct termios saved_attr;
void set_input_mode(void) {
  struct termios tattr;
  if(tcgetattr(STDIN_FILENO, &saved_attr) == -1) {
    fprintf(stderr, "tcgetattr: %s", strerror(errno));
    exit(1);
  }
  if(tcgetattr(STDIN_FILENO, &tattr) == -1) {
    fprintf(stderr, "tcgetattr: %s", strerror(errno));
    exit(1);
  }
  tattr.c_iflag = ISTRIP;
  tattr.c_oflag = 0;
  tattr.c_lflag = 0;
  if(tcsetattr(STDIN_FILENO, TCSANOW, &tattr) == -1) {
    fprintf(stderr, "tcsetattr: %s", strerror(errno));
    exit(1);
  }
}

void reset_input_mode(void) {
  if(tcsetattr(STDIN_FILENO, TCSANOW, &saved_attr) == -1) {
    fprintf(stderr, "tcsetattr: %s", strerror(errno));
    exit(1);
  }
}


int main(int argc, char** argv) {

  int port_flag = 0;
  int log_flag = 0;
  int compress_flag = 0;
  int opt;
  struct option long_options[4] = {{"port", required_argument, NULL, 'p'},
				   {"log", required_argument, NULL, 'l'},
				   {"compress", no_argument, NULL, 'c'},
				   {0, 0, 0, 0}};
  int ofd;
  int portno;
  z_stream def_strm;
  z_stream inf_strm;
  char* logfile = NULL;
  while(1) {
    opt = getopt_long(argc, argv, ":", long_options, NULL);
    if(opt == -1)
      break;
    switch(opt) {
    case 'p':
      port_flag = 1;
      portno = atoi(optarg);
      break;
    case 'l':
      log_flag = 1;
      logfile = malloc(sizeof(char)*(strlen(optarg)+1));
      strcpy(logfile, optarg);
      break;
    case 'c':
      compress_flag = 1;
      break;
    case '?':
      fprintf(stderr, "Unrecognized argument: %s\n"
	      "Correct usage: %s --port=portno (--log=filename) (--compress)", argv[optind-1], argv[0]);
      free(logfile);
      exit(1);
      break;
    case ':':
      fprintf(stderr, "Missing argument for %s\n"
	      "Correct usage: %s --port=portno (--log=filename) (--compress)", argv[optind-1], argv[0]);
      free(logfile);
      exit(1);
      break;
    }
  }

  if(optind != argc)
  {
    fprintf(stderr, "Unrecognized argument: %s\n"
	    "Correct usage: %s --port=portno (--log=filename) (--compress)", argv[optind], argv[0]);
    free(logfile);
    exit(1);
  }

  if(log_flag) {
    ofd = creat(logfile, 0666);
    free(logfile);
    if(ofd == -1) {
      fprintf(stderr, "cannot create logfile");
      exit(1);
    }
  }
 
  if(!port_flag) {
    fprintf(stderr, "need to specify port number");
    free(logfile);
    exit(1);
  }



  if(compress_flag) {
    // initialize deflation
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    if(deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
      fprintf(stderr, "Error on deflateInit");
      exit(1);
    }
    // initialize inflation
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    inf_strm.avail_in = 0;
    inf_strm.next_in = Z_NULL;
    if(inflateInit(&inf_strm) != Z_OK) {
      fprintf(stderr, "Error on inflateInit");
      exit(1);
    }
  }
  set_input_mode();

  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[BUFSIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    fprintf(stderr, "error opening socket\n");
    reset_input_mode();
    exit(1);
  }
  server = gethostbyname("localhost");
  if(server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    reset_input_mode();
    exit(1);
  }
  bzero((char*) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char*) server->h_addr,
	(char*)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "error connecting\n");
    reset_input_mode();
    exit(1);
  }

  // by here, a connection is made
  
  int exit_client = 0;
  struct pollfd fds[2];
  fds[0].fd = STDIN_FILENO; // read from keyboard
  fds[0].events = POLLIN | POLLHUP | POLLERR;
  fds[1].fd = sockfd; // read from TCP socket
  fds[1].events = POLLIN | POLLHUP | POLLERR;

  bzero(buffer, BUFSIZE);
  
  while(1) {
    int ret = poll(fds, 2, 0);
    if(ret == -1) {
      fprintf(stderr, "poll: %s", strerror(errno));
      reset_input_mode();
      exit(1);
    }
    
    if(ret > 0) {
      if(fds[0].revents & POLLIN) { // read input from the keyboard
	n = read(fds[0].fd, buffer, BUFSIZE);
	if(n == -1) {
	  fprintf(stderr, "read: %s", strerror(errno));
	  reset_input_mode();
	  exit(1);
	}
	for(int i = 0; i < n; i++) {
	  if(buffer[i] == '\r' || buffer[i] == '\n') {
	    if(write(STDOUT_FILENO, "\r\n", 2) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      reset_input_mode();
	      exit(1);
	    }
	  }
	  else if(buffer[i] == 0x03) {
	    if(write(STDOUT_FILENO, "^C", 2) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      reset_input_mode();
	      exit(1);
	    }
	  }
	  else if(buffer[i] == 0x04) {
	    if(write(STDOUT_FILENO, "^D", 2) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      reset_input_mode();
	      exit(1);
	    }
	  }
	  else {
	    if(write(STDOUT_FILENO, &buffer[i], 1) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      reset_input_mode();
	      exit(1);
	    }
	  }
	}
	if(compress_flag) {
	  int have;
	  char out[BUFSIZE];

	  def_strm.avail_in = n;
	  def_strm.next_in = (unsigned char*)buffer;
	  def_strm.avail_out = BUFSIZE;
	  def_strm.next_out = (unsigned char*)out;
	  do {
	    deflate(&def_strm, Z_SYNC_FLUSH);
	  } while (def_strm.avail_in > 0);
	  have = BUFSIZE - def_strm.avail_out;
	  write(sockfd, out, have);
	  if(log_flag) {
	    char s[20];
	    int l = sprintf(s, "%d", have);
	    write(ofd, "SENT ", 5);
	    write(ofd, s, l);
	    write(ofd, " bytes: ", 8);
	    write(ofd, out, have);
	    write(ofd, "\n", 1);
	  }
	}
	else {
	  if(write(sockfd, buffer, n) == -1) {
	    fprintf(stderr, "write: %s", strerror(errno));
	    reset_input_mode();
	    exit(1);
	  }
	  if(log_flag) {
	    char s[20];
	    int l = sprintf(s, "%d", n);
	    write(ofd, "SENT ", 5);
	    write(ofd, s, l);
	    write(ofd, " bytes: ", 8);
	    write(ofd, buffer, n);
	    write(ofd, "\n", 1);
	  }
	}

      }
      
      if(fds[1].revents & POLLIN) { //  read input from the TCP socket
	n = read(sockfd, buffer, BUFSIZE);
	if(n == -1) {
	  fprintf(stderr, "read: %s", strerror(errno));
	  reset_input_mode();
	  exit(1);
	}
	if(n == 0) { // if the other end of socket closes, read will return 0
	   if(compress_flag) {
	     deflateEnd(&def_strm);
	     inflateEnd(&inf_strm);
	   }
	   reset_input_mode();
	   exit(0);
	}
	if(compress_flag) {
	  int have;
          char out[BUFSIZE*4];

	  inf_strm.avail_in = n;
	  inf_strm.next_in = (unsigned char*)buffer;
	  inf_strm.avail_out = BUFSIZE*4;
	  inf_strm.next_out = (unsigned char*)out;
	  do {
	    inflate(&inf_strm, Z_SYNC_FLUSH);
	  } while (inf_strm.avail_in > 0);
	  have = BUFSIZE*4 - inf_strm.avail_out;

	  for(int i = 0; i < have; i++) {
	    if(out[i] == '\n') { // if receive <lf> from shell, print as <cr><lf>
	      if(write(STDOUT_FILENO, "\r\n", 2) == -1) {
		fprintf(stderr, "write: %s", strerror(errno));
		reset_input_mode();
		exit(1);
	      }
	    }
	    else {
	      if(write(STDOUT_FILENO, &out[i], 1) == -1) {
		fprintf(stderr, "write: %s", strerror(errno));
		reset_input_mode();
		exit(1);
	      }
	    }
	  }
	}
	else {
	  for(int i = 0; i < n; i++) {
	    if(buffer[i] == '\n') { // if receive <lf> from shell, print as <cr><lf>
	      if(write(STDOUT_FILENO, "\r\n", 2) == -1) {
		fprintf(stderr, "write: %s", strerror(errno));
		reset_input_mode();
		exit(1);
	      }
	    }
	    else {
	      if(write(STDOUT_FILENO, &buffer[i], 1) == -1) {
		fprintf(stderr, "write: %s", strerror(errno));
		reset_input_mode();
		exit(1);
	      }
	    }
	  }
	}
		
	if(log_flag) {
	  char s[20];
	  int l = sprintf(s, "%d", n);
	  write(ofd, "RECEIVED ", 9);
	  write(ofd, s, l);
	  write(ofd, " bytes: ", 8);
	  write(ofd, buffer, n);
	  write(ofd, "\n", 1);
	}
      }
      
      if((fds[1].revents & POLLHUP) || (fds[1].revents & POLLERR)) {
	exit_client = 1;
      }
    }
    
    if(exit_client) {
      if(compress_flag) {
	deflateEnd(&def_strm);
	inflateEnd(&inf_strm);
      }
      reset_input_mode();
      exit(0);
    }
  }
  
}
