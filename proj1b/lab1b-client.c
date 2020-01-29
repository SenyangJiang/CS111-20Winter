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

  if(argc < 2) {
    fprintf(stderr, "too few arguments, need to specify port\n");
    exit(1);
  }
  int opt;
  struct option long_options[4] = {{"port", required_argument, NULL, 'p'},
				   {"log", required_argument, NULL, 'l'},
				   {"compress", no_argument, NULL, 'c'},
				   {0, 0, 0, 0}};
  int portno;
  
  while(1) {
    opt = getopt_long(argc, argv, ":", long_options, NULL);
    if(opt == -1)
      break;
    switch(opt) {
    case 'p':
      portno = atoi(optarg);
      break;
    case 'l':
      break;
    case 'c':
      break;
    case '?':
      fprintf(stderr, "Unrecognized argument: %s\n",  argv[optind-1]);
      exit(1);
      break;
    case ':':
      fprintf(stderr, "Missing argument for %s\n", argv[optind-1]);
      exit(1);
      break;
    }
  }

  if(optind != argc)
  {
    fprintf(stderr, "Unrecognized argument: %s\n", argv[optind]);
    exit(1);
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
    //fprintf(stdout, "%d", ret);
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
	    if(write(sockfd, "\n", 1) == -1) {
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
	    if(write(sockfd, &buffer[i], 1) == -1) {
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
	    if(write(sockfd, &buffer[i], 1) == -1) {
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
	    if(write(sockfd, &buffer[i], 1) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      reset_input_mode();
	      exit(1);
	    }
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
	for(int i = 0; i < n; i++) {
	  if(buffer[i] == '\n') {
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
      
      if((fds[1].revents & POLLHUP) || (fds[1].revents & POLLERR)) {
	exit_client = 1;
      }
    }
    
    if(exit_client) {
      reset_input_mode();
      exit(0);
    }
  }
  
}
