#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
// #include<sys/syscall.h>

int main(int argc, char* argv[]) {

  /* Check operands */
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of operands.");
    exit(1);
  }

  int lengthOfFrom = strlen(argv[1]);
  int lengthOfTo = strlen(argv[2]);
  
  /* Check lengths of byte strings. */
  if (lengthOfFrom != lengthOfTo) {
    fprintf(stderr, "FROM and TO are not the same length.");
    exit(1);
  }

  /* Check duplicate bytes. */
  int i,j;
  i=0;
  for (i; i<lengthOfFrom; i++) {
    j=i+1;
    for (j; j<lengthOfTo; j++) {
      if (argv[1][i] == argv[1][j]) {
	fprintf(stderr, "FROM has duplicate bytes.");
	exit(1);
      }
    }
  }

  /* Translate. */
  char c = getchar();
  while (c != EOF) {
    if (ferror(stdin)) {
      fprintf(stderr, "Fail to read from input.");
      exit(1);
    }
    i=0;
    for (i; i<lengthOfFrom; i++) {
      if (argv[1][i] == c) {
	putchar(argv[2][i]);
	if (ferror(stdout)) {
	  fprintf(stderr, "Fail to write to output.");
	  exit(1);
	}
	break;
      }
    }
    if (i == lengthOfFrom) {
      putchar(c);
      if (ferror(stdout)) {
	fprintf(stderr, "Fail to write to output.");
	exit(1);
      }
    }
    c = getchar();
  }
  return 0;
}
