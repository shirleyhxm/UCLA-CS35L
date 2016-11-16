#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

#include<sys/stat.h>
#include<sys/types.h>

int count = 0;

int frobcmp(const void  *a, const void *b){
  count++;
  char* aa = *(char **) a;
  char* bb = *(char **) b;
  int i;
  for (i = 0; aa[i] != ' ' && bb[i] != ' '; i++) {
    // unfrob_a = *(unsigned char *)a ^ 42;
    // unfrob_b = *(unsigned char *)b ^ 42;
    char unfrob_aa = aa[i] ^ 42;
    char unfrob_bb = bb[i] ^ 42;
    if (unfrob_aa !=  unfrob_bb) {
      return (unfrob_aa - unfrob_bb);
    }
  }
  if (aa[i] != ' ')
    return 1;
  if (bb[i] != ' ')
    return -1;
  return 0;
}

int main(void){
  int array_size = 100;
  int string_size = 50; 
  int array_i = 0;
  int char_i = 0;
  
  /* Retrieve file information */
  struct stat st;
  if (fstat(0,&st) < 0) {
    fprintf(stderr, "Fail to retrive file information.");
    exit(1);
  }

  if (ferror(stdin)) {
    fprintf(stderr, "Fail to read from standard input.");
    exit(1);
  }
  
  /* Check if file is regular, and determine the size */
  if (S_ISREG(st.st_mode) && st.st_size > 0) {
    array_size = st.st_size + 1;
  }

  /* Allocate 1st dimension space */
  char** ptr = (char**)malloc(array_size * sizeof(char*));  
  if (ptr == NULL) {
    fprintf(stderr, "Fail to allocate memory.");
    exit(1);
  }

  /* Allocate 2nd dimension space */
  ptr[array_i] = (char*)malloc(string_size*sizeof(char));
  if (ptr[array_i] == NULL) {
    fprintf(stderr, "Fail to allocate memory.");
    exit(1);
  }

  /* Read */
  char c;

  while (read(0, &c, 1) > 0){
    ptr[array_i][char_i] = c;

    if (ferror(stdin)) {
      fprintf(stderr, "Fail to read from input.");
      exit(1);
    }
    
    /* end of a word */
    if (c == ' ') {
      array_i++;
      /* reallocate array */ 
      if (array_i >= array_size) {
	array_size *= 2;
	ptr = (char **)realloc(ptr, array_size*sizeof(char*));
	if (ptr == NULL) {
	  fprintf(stderr, "Fail to reallocate memory.");
	  exit(1);
	}
      }
      char_i = 0;
      /* allocate space for next word */
      ptr[array_i] = (char*)malloc(string_size*sizeof(char));
      if (ptr[array_i] == NULL) {
	fprintf(stderr, "Fail to allocate memory.");
	exit(1);
      }
    }
    
    /* continue reading from word */
    else if (c != ' ') {
      char_i++;
      if (char_i >= string_size) {
	string_size *= 2;
	ptr[array_i] = (char *)realloc(ptr[array_i],string_size*sizeof(char));
      }
    }
  }

  /* Append a space at the end if needed. */
  if (char_i > 0 && ptr[array_i][char_i-1] != ' ') {
    ptr[array_i][char_i] = ' ';
  }

  /* Sort recorded array of words */
  qsort(ptr, (size_t)(array_i+1), (size_t)sizeof(char*), frobcmp);

  /* Write sorted words */
  int i,j;
  for (i = 0; i < array_i+1; i++) {
    for (j = 0; j < string_size; j++) {
      write(1, &ptr[i][j], 1);
      if (ferror(stdout)) {
	fprintf(stderr, "Fail to write to output.");
	exit(1);
      }
      if (ptr[i][j] == ' ') {
	break;
      }	
    }
  }
  
  /* Free allocated space */
  for (i = 0; i < array_i+1; i++) {
      free(ptr[i]);
  }
  free(ptr);
  fprintf(stderr, "Comparisons: %d\n", count);
  return 0;
}

