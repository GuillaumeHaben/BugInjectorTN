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
  int one = 1;
  int zero = 0;

  int *ptr_one = NULL;
  int *ptr_two = &one;
  int *ptr_three = NULL;
  int **ptr_four = &ptr_one;

  *ptr_three = 2;

  //ptr_one = (int *) malloc(sizeof(int));
  ptr_two = (int *) malloc(sizeof(int));
  //ptr_three = (int *) malloc(sizeof(int));

  // TRUE
  while (ptr_one) {

  }

  while (ptr_three) {
    if(1==1) {
    }
  }

  while (ptr_four) {

  }

  while (NULL) {

  }

  // FALSE

  while (ptr_two) {

  }

  while (zero) {

  }

  while (one) {

  }

  while (1 == 1) {

  }

  return 0;
}
