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
#include <string.h>

int
main(void)
{
  int foo;
  int *foo_ptr = &foo;
  char foo1;
  char *foo1_ptr = &foo1;

  char src[40];
  char dest[100];

  // FALSE

  int a = 0;
  int n[10];
  char arr[3];
  int *buffer = malloc(2 + sizeof(int));
  int doubleArray[10][10];
  int array[] = {'1','2','3','4','5'};

  // TRUE
if (1==1) {

}
  buffer[0] = 1;
  n[2] = 0;
  arr[0] = 'a';
  arr[1] = 'b';
  arr[2] = 'c';
  doubleArray[2][2] = 15;
  buffer[n[2]] = 0;
  *(arr + 2) = 'x';
  *(arr - 2) = 'x';
  *(arr) = 'x';
  *((int *)doubleArray + 10*2 + 2) = 15;
  *(foo_ptr + 2) = 42;
  *(foo1_ptr + 2) = 'd';

  memset(dest, '\0', sizeof(dest));
  strcpy(src, "Hello world !");
  strcpy(dest, src);

  return 0;
}
