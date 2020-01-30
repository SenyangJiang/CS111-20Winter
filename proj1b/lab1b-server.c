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
#include "zlib.h"

#define BUFSIZE 2048

int main(int argc, char** argv) {

  int port_flag = 0;
  int compress_flag = 0;
  int opt;
  struct option long_options[3] = {{"port", required_argument, NULL, 'p'},
				   {"compress", no_argument, NULL, 'c'},
				{0, 0, 0, 0}};

  int portno;
  z_stream def_strm;
  z_stream inf_strm;
  while(1) {
    opt = getopt_long(argc, argv, ":", long_options, NULL);
    if(opt == -1)
      break;
    switch(opt) {
    case 'p':
      port_flag = 1;
      portno = atoi(optarg);
      break;
    case 'c':
      compress_flag = 1;
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

  if(!port_flag) {
    fprintf(stderr, "need to specify port number");
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
	  if(compress_flag) {
	    int have;
	    char out[BUFSIZE];
	    
	    inf_strm.avail_in = n;
	    inf_strm.next_in = (unsigned char*)buffer;
	    inf_strm.avail_out = BUFSIZE;
	    inf_strm.next_out = (unsigned char*)out;
	    do {
	      inflate(&inf_strm, Z_SYNC_FLUSH);
	    } while (inf_strm.avail_in > 0);
	    have = BUFSIZE - inf_strm.avail_out;
	    for(int i = 0; i < have; i++) {
	      if(out[i] == 0x03) {
		if(kill(c_pid, SIGINT) == -1) {
		  fprintf(stderr, "kill: %s", strerror(errno));
		  exit(1);
		}
	      }
	      else if(out[i] == 0x04) {
		close(pipefd_in[1]);
	      }
	      else if(out[i] == '\r') { // <cr> goes into shell as <lf> 
		if(write(pipefd_in[1], "\n", 1) == -1) {
		  fprintf(stderr, "write: %s", strerror(errno));
		  exit(1);
		}
	      }
	      else {
		if(write(pipefd_in[1], &out[i], 1) == -1) {
		  fprintf(stderr, "write: %s", strerror(errno));
		  exit(1);
		}
	      }
	    }
	  }
	  else {
	    for(int i = 0; i < n; i++) {
	      if(buffer[i] == 0x03) {
		if(kill(c_pid, SIGINT) == -1) {
		  fprintf(stderr, "kill: %s", strerror(errno));
		  exit(1);
		}
	      }
	      else if(buffer[i] == 0x04) {
		close(pipefd_in[1]);
	      }
	      else if(buffer[i] == '\r') { // <cr> goes into shell as <lf>
		if(write(pipefd_in[1], "\n", 1) == -1) {
		  fprintf(stderr, "write: %s", strerror(errno));
		  exit(1);
		}
	      }
	      else {
		if(write(pipefd_in[1], &buffer[i], 1) == -1) {
		  fprintf(stderr, "write: %s", strerror(errno));
		  exit(1);
		}
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
	    if(write(newsockfd, out, have) == -1) {
	      fprintf(stderr, "write: %s", strerror(errno));
	      exit(1);
	    }
	  }
	  else {
	    if(write(newsockfd, buffer, n) == -1) {
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
	  if(compress_flag) {
	    deflateEnd(&def_strm);
	    inflateEnd(&inf_strm);
	  }
	  exit(0);
      }
    }
  }
  
}
