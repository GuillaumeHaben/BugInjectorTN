/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
  int *ptr_one;
  int *ptr_two;
  int *x = NULL;

  ptr_one = (int *) malloc(sizeof(int));
  ptr_two = (int *) malloc(sizeof(int));

  if (ptr_one == 0 || ptr_two == 0) {
    printf("ERROR: Out of memory\n");
    return 1;
  }

  *ptr_one = 25;
  *ptr_two = 10;

  printf("%d:%d\n", *ptr_one, *ptr_two);

  free(ptr_one);
  free(ptr_two);

  return 0;
}
