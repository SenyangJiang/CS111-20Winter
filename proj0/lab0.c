// NAME: Senyang Jiang
// EMAIL: senyangjiang@yahoo.com
// ID: 505111806

#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void sig_handler(int signo)
{
  fprintf(stderr , "Error from --segfault:\n"
	  "forced segmentation fault by dereferencing a NULL pointer\n"
	  "catched by --catch option\n"
	  "signal number: %d\n", signo);
  exit(4);
}

int main(int argc, char **argv)
{
  int opt;
  struct option long_options[] = {{"input", required_argument, NULL, 'i'},
			 {"output", required_argument, NULL, 'o'},
			 {"segfault", no_argument, NULL, 's'},
			 {"catch", no_argument, NULL, 'c'},
			 {0, 0, 0, 0}};
  int input = 0;
  int output = 0;
  int segfault = 0;
  int catch = 0;
  char* input_file = NULL;
  char* output_file = NULL;

  //printf("%d\n", optind);
  while(1)
    {
      opt = getopt_long(argc, argv, "+:", long_options, NULL);
      //printf("%d\n", optind);
      if(opt == -1)
	break;
      
      switch(opt)
	{
	case 'i':
	  input = 1;
	  input_file = malloc(sizeof(char)*(strlen(optarg)+1));
	  strcpy(input_file, optarg);
	  break;
	case 'o':
	  output = 1;
	  output_file = malloc(sizeof(char)*(strlen(optarg)+1));
	  strcpy(output_file, optarg);
	  break;
	case 's':
	  segfault = 1;
	  break;
	case 'c':
	  catch = 1;
	  break;
	case '?':
	  fprintf(stderr, "Unrecognized argument: %s\n"
		  "Correct usage: %s (--input=filename) (--output=filename) (--segfault) (--catch)\n", argv[optind-1], argv[0]);
	  free(input_file);
	  free(output_file);
	  exit(1);
	  break;
	case ':':
	  fprintf(stderr, "Missing argument for %s\n"
		  "Correct Usage: %s (--input=filename) (--output=filename) (--segfault) (--catch)\n", argv[optind-1], argv[0]);
	  free(input_file);
	  free(output_file);
	  exit(1);
	  break;
	}
    }
  // check if there is an element that is not an option
  if(optind != argc)
    {
      fprintf(stderr, "Unrecognized argument: %s\n"
		  "Correct usage: %s (--input=filename) (--output=filename) (--segfault) (--catch)\n", argv[optind], argv[0]);
      free(input_file);
      free(output_file);
      exit(1);
    }
  
  if(input)
    {
      int ifd = open(input_file, O_RDONLY);
      if(ifd >= 0)
	{
	  close(0);
	  dup(ifd);
	  close(ifd);
	  free(input_file);
	}
      else
	{
	  fprintf(stderr, "Error from --input:\n"
		  "File '%s' cannot be opened\n"
		  "Reason: %s\n", input_file, strerror(errno));
	  free(input_file);
	  free(output_file);
	  exit(2);
	}
    }
  
  if(output)
    {
      int ofd = creat(output_file, 0666);
      if(ofd >= 0)
	{
	  close(1);
	  dup(ofd);
	  close(ofd);
	  free(output_file);
	}
      else
	{
	  fprintf(stderr, "Error from --output:\n"
		  "File '%s' cannot be created\n"
		  "Reason: %s\n", output_file, strerror(errno));
	  free(output_file);
	  exit(3);
	}
    }

  if(catch)
    {
      signal(SIGSEGV, sig_handler);
    }

  if(segfault)
    {
      char* danger = NULL;
      *danger = 'a';
    }
  
  char c;
  while(read(0, &c, 1) > 0){
    write(1, &c, 1);
  }
  exit(0);
}
