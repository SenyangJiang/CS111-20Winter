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

#define BUFSIZE 256
struct termios saved_attr;
void set_input_mode(void) {
  struct termios tattr;
  tcgetattr(STDIN_FILENO, &saved_attr);
  tcgetattr(STDIN_FILENO, &tattr);
  tattr.c_iflag = ISTRIP;
  tattr.c_oflag = 0;
  tattr.c_lflag = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void reset_input_mode(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_attr);
}

int main(int argc, char** argv) {
  
  int opt;
  struct option long_options[2] = {{"shell", no_argument, NULL, 's'},
				{0, 0, 0, 0}};
  int shell = 0;

  while(1) {
    opt = getopt_long(argc, argv, ":", long_options, NULL);
    if(opt == -1)
      break;
    switch(opt) {
    case 's':
      shell = 1;
      break;
    case '?':
      fprintf(stderr, "Unrecognized argument: %s\n",  argv[optind-1]);
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
  
  char buffer[BUFSIZE];
  int n;
  
  if(shell) {
    int pipefd_in[2]; // to shell
    int pipefd_out[2]; // to terminal process
    if(pipe(pipefd_in) == -1) {
      fprintf(stderr, strerror(errno));
      exit(1);
    }
    if(pipe(pipefd_out) == -1) {
      fprintf(stderr, strerror(errno));
      exit(1);
    }
    pid_t c_pid = fork();
    if(c_pid == -1) {
      fprintf(stderr, strerror(errno));
      exit(1);
    }
    if(c_pid == 0) {
      close(pipefd_in[1]);
      close(pipefd_out[0]);
      // set standard input to be the pipe from the terminal process
      close(0);
      dup(pipefd_in[0]);
      close(pipefd_in[0]);
      // set standard output/error to be the pipe to the terminal process
      close(1);
      close(2);
      dup(pipefd_out[1]);
      dup(pipefd_out[1]);
      close(pipefd_out[1]);
      
      if(execl("/bin/bash", "", (char*)NULL) == -1) {
	fprintf(stderr, strerror(errno));
	exit(1);
      }
    }
    else {
      close(pipefd_in[0]);
      close(pipefd_out[1]);
      struct pollfd fds[2];
      fds[0].fd = STDIN_FILENO;
      fds[0].events = POLLIN | POLLHUP | POLLERR;
      fds[1].fd = pipefd_out[0];
      fds[1].events = POLLIN | POLLHUP | POLLERR;
      int exit_shell = 0;
      int status;
      while(1) {
	int ret = poll(fds, 2, 0);
	if(ret == -1) {
	  fprintf(stderr, strerror(errno));
	  exit(1);
	}
	if(ret > 0) {
	  if(fds[0].revents & POLLIN) { // read input from the keyboard
	    n = read(fds[0].fd, buffer, BUFSIZE);
	    for(int i = 0; i < n; i++) {
	      if(buffer[i] == '\r' || buffer[i] == '\n') {
		write(STDOUT_FILENO, "\r\n", 2);
		write(pipefd_in[1], "\n", 1);
	      }
	      else if(buffer[i] == 0x03) {
		write(pipefd_in[1], &buffer[i], 1);
		write(STDOUT_FILENO, "^C", 2);
		if(kill(c_pid, SIGINT) == -1) {
		  fprintf(stderr, strerror(errno));
		  exit(1);
		}
	      }
	      else if(buffer[i] == 0x04) {
		close(pipefd_in[1]);
		write(STDOUT_FILENO, "^D", 2);
	      }
	      else {
		write(STDOUT_FILENO, &buffer[i], 1);
		write(pipefd_in[1], &buffer[i], 1);
	      }
	    }
	  }
	  if(fds[1].revents & POLLIN) { //  read input from the shell pipe
	    n = read(fds[1].fd, buffer, BUFSIZE);
	    for(int i = 0; i < n; i++) {
	      if(buffer[i] == '\n') {
		write(STDOUT_FILENO, "\r\n", 2);
	      }
	      else {
		write(STDOUT_FILENO, &buffer[i], 1);
	      }
	    }
	  }
	  if((fds[1].revents & POLLHUP) || (fds[1].revents & POLLERR)) {
	    exit_shell = 1;
	  }
	}
	
	if(exit_shell) {
	    close(pipefd_out[0]);
	    waitpid(c_pid, &status, 0);
	    fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d", status&0x007f, (status&0xff00)>>8);
	    reset_input_mode();
	    exit(0);
	}
      }
    }
  }
  
  while(1) {
    n = read(STDIN_FILENO, buffer, BUFSIZE);
    for(int i = 0; i < n; i++) {
      if(buffer[i] == '\r' || buffer[i] == '\n') {
	write(STDOUT_FILENO, "\r\n", 2);
      }
      else {
	write(STDOUT_FILENO, &buffer[i], 1);
      }
      if(buffer[i] == 0x04) {
	reset_input_mode();
	exit(0);
      }
    }
  }
}
