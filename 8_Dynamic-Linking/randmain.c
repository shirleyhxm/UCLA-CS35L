#include"randcpuid.h"
#include<immintrin.h>
#include<dlfcn.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */

  unsigned long long (*rand64) (void);
  void *handle;
  char *err;
  if (rdrand_supported ())
    {
      printf("HW\n");
      handle = dlopen("./randlibhw.so", RTLD_NOW);
      if (!handle) {
	printf("A dynamic linking error occurred while opening randlibhw.so : (%s)\n", dlerror());
	return 1;
      }
      rand64 = dlsym(handle, "hardware_rand64");
      err = dlerror();
      if (err != NULL) {
	printf("A dynamic linking error with randlibhw.so occurred: (%s)\n", err);
	return 1;
      }
    }
  else
    {
      printf("SW\n");
      handle = dlopen("./randlibsw.so", RTLD_NOW);
      if (!handle) {
	printf("A dynamic linking error occurred while opening randlibsw.so : (%s)\n", dlerror());
	return 1;
      }
      rand64 = dlsym(handle, "software_rand64");
      err = dlerror();
      if (err != NULL) {
	printf("A dynamic linking error with randlibsw.so occurred: (%s)\n", err);
	return 1;
      }
    }

  int wordsize = sizeof rand64();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      /*
      if (fwrite (&x, 1, outbytes, stdout) != outbytes)
	{
	  output_errno = errno;
	  break;
	}
      */
      printf("%llu\n",x);
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (dlclose(handle) != 0) {
    printf("An error occurred while closing a dynamic libray: (%s)\n", dlerror());
    return 1;
  }
  
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }

  
  return 0;
}
