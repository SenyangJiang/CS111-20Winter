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
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 2048

int main(int argc, char** argv) {
  if(argc < 2) {
    fprintf(stderr, "too few arguments, need to specify port\n");
    exit(1);
  }
  int opt;
  struct option long_options[2] = {{"port", required_argument, NULL, 'p'},
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
  
  int sockfd, newsockfd, n;
  socklen_t cli_len;
  char buffer[BUFSIZE];
  struct sockaddr_in serv_addr, cli_addr;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    fprintf(stderr, "error opening socket\n");
    exit(1);
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "error on binding\n");
    exit(1);
  }
  listen(sockfd, 5);
  cli_len = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
  if(newsockfd < 0) {
    fprintf(stderr, "error on accept\n");
    exit(1);
  }
  
  // by here, a connection is made
  
  bzero(buffer, BUFSIZE);

  int pipefd_in[2]; // to shell
  int pipefd_out[2]; // to server
  if(pipe(pipefd_in) == -1) {
    fprintf(stderr, "pipe: %s", strerror(errno));
    exit(1);
  }
  if(pipe(pipefd_out) == -1) {
    fprintf(stderr, "pipe: %s", strerror(errno));
    exit(1);
  }
  pid_t c_pid = fork();
  if(c_pid == -1) {
    fprintf(stderr, "fork: %s", strerror(errno));
    exit(1);
  }
  if(c_pid == 0) {
    close(pipefd_in[1]);
    close(pipefd_out[0]);
    // set standard input to be the pipe from the server
    close(0);
    dup(pipefd_in[0]);
    close(pipefd_in[0]);
    // set standard output/error to be the pipe to the server
    close(1);
    close(2);
    dup(pipefd_out[1]);
    dup(pipefd_out[1]);
    close(pipefd_out[1]);

    if(execl("/bin/bash", "", (char*)NULL) == -1) {
      fprintf(stderr, "execl: %s", strerror(errno));
      exit(1);
    }
  }
  else {
    close(pipefd_in[0]);
    close(pipefd_out[1]);
    struct pollfd fds[2];
    fds[0].fd = newsockfd;
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].fd = pipefd_out[0];
    fds[1].events = POLLIN | POLLHUP | POLLERR;
    int exit_shell = 0;
    int status;
    while(1) {
      int ret = poll(fds, 2, 0);
      if(ret == -1) {
	fprintf(stderr, "poll: %s", strerror(errno));
	exit(1);
      }
      if(ret > 0) {
	if(fds[0].revents & POLLIN) { // read input from the TCP socket
	  n = read(fds[0].fd, buffer, BUFSIZE);
	  if(n == -1) {
	    fprintf(stderr, "read: %s", strerror(errno));
	    exit(1);
	  }
	  for(int i = 0; i < n; i++) {
	    if(buffer[i] == 0x03) {
	      if(write(pipefd_in[1], &buffer[i], 1) == -1) {
		fprintf(stderr, "write: %s", strerror(errno));
		exit(1);
	      }
	      if(kill(c_pid, SIGINT) == -1) {
		fprintf(stderr, "kill: %s", strerror(errno));
		exit(1);
	      }
	    }
	    else if(buffer[i] == 0x04) {
	      close(pipefd_in[1]);
	    }
	    else {
	      if(write(pipefd_in[1], &buffer[i], 1) == -1) {
		fprintf(stderr, "write: %s", strerror(errno));
		exit(1);
	      }
	    }
	  }
	}
	if(fds[1].revents & POLLIN) { //  read input from the shell pipe
	  n = read(fds[1].fd, buffer, BUFSIZE);
	  if(n == -1) {
	    fprintf(stderr, "read: %s", strerror(errno));
	    exit(1);
	  }
	  for(int i = 0; i < n; i++) {
	    if(write(newsockfd, &buffer[i], 1) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      exit(1);
	    }
	  }
	}
	if((fds[1].revents & POLLHUP) || (fds[1].revents & POLLERR)) {
	  exit_shell = 1;
	}
	if((fds[0].revents & POLLHUP) || (fds[0].revents & POLLERR)) {
	  close(pipefd_in[1]);
	}
      }

      if(exit_shell) {
	  close(pipefd_out[0]);
	  if(waitpid(c_pid, &status, 0) == -1) {
	    fprintf(stderr, "waitpid: %s", strerror(errno));
	    exit(1);
	  }
	  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d", status&0x007f, (status&0xff00)>>8);
	  close(sockfd);
	  close(newsockfd);
	  exit(0);
      }
    }
  }
  
}
