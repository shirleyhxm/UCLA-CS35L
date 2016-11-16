/* 
 * sfrob.c
 */ 

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int frobcmp(const void  *a, const void *b){
  //unsigned char unfrob_a;
  //unsigned char unfrob_b;
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
  int c = getchar();

  if (ferror(stdin)) {
    fprintf(stderr, "Fail to read from standard input.");
    exit(1);
  }
  
  if ((char)c == EOF) {
    return 0;
  }

  char** ptr = (char**)malloc(array_size * sizeof(char*));  
  if (ptr == NULL) {
    fprintf(stderr, "Fail to allocate memory.");
    exit(1);
  }

  ptr[array_i] = (char*)malloc(string_size*sizeof(char));
  if (ptr[array_i] == NULL) {
    fprintf(stderr, "Fail to allocate memory.");
    exit(1);
  }
  
  while ((char)c != EOF){
    ptr[array_i][char_i] = (char)c;
    int next = getchar();

    if (ferror(stdin)) {
      fprintf(stderr, "Fail to read from standard input.");
      exit(1);
    }
    
    /* end of a word */
    if ((char)c == ' ' && (char)next != EOF) {
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
    else if ((char)c != ' ') {
      char_i++;
      if (char_i >= string_size) {
	string_size *= 2;
	// char * old_stringptr = ptr[array_i];
	ptr[array_i] = (char *)realloc(ptr[array_i],string_size*sizeof(char));
	// free(old_stringptr); 
      }
      if ((char)next == EOF) {
	 ptr[array_i][char_i] = ' ';
      }
    }
    c = next;
  }
  
  /* sort recorded array of words */
  qsort(ptr, (size_t)(array_i+1), (size_t)sizeof(char*), frobcmp);
  
  /* output sorted words */
  int i,j;
  for (i = 0; i < array_i+1; i++) {
    for (j = 0; j < string_size; j++) {
      putchar(ptr[i][j]);
      if (ferror(stdout)) {
	fprintf(stderr, "Fail to write to standard output.");
	exit(1);
      }
      if (ptr[i][j] == ' ') {
	break;
      }	
    }
  }
  
  /* free allocated space */
  for (i = 0; i < array_i+1; i++) {
    //if (ptr[i] != NULL) {
      free(ptr[i]);
      //}
  }
  free(ptr);
  return 0;
}

