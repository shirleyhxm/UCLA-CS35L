#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
//#include<sys/syscall.h>

int main(int argc, char* argv[]) {

  /* Check operands */
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of operands.");
    exit(1);
  }
  
  /* Check string lengths. */
  if (strlen(argv[1]) != strlen(argv[2])) {
    fprintf(stderr, "FROM and TO are not the same length.");
    exit(1);
  }

  /* Check duplicate bytes. */
  int i,j;
  for (i=0; i<strlen(argv[1]); i++) {
    for (j=i+1; j<strlen(argv[1]); j++) {
      if (argv[1][i] == argv[1][j]) {
	fprintf(stderr,"FROM has duplicate bytes.");
	exit(1);
      }
    }
  }

  /* Translate. */
  char c;
  while (read(0, &c, 1) > 0) {
    if (ferror(stdin)) {
      fprintf(stderr, "Fail to read from input.");
      exit(1);
    }
      
    for (i=0; i<strlen(argv[1]); i++) {
      if (c == argv[1][i]) {
	write(1, &argv[2][i], 1);
	if (ferror(stdout)) {
	  fprintf(stderr, "Fail to write to output.");
	  exit(1);
	}
	break;
      }
    }
    
    if (i == strlen(argv[1])) {
      write(1, &c, 1);
      if (ferror(stdout)) {
	fprintf(stderr, "Fail to write to output.");
	exit(1);
      }
    }
    
  }
  return 0;
}
