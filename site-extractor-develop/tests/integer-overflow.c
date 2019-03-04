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
  int b = 0;
  b += 2;
  int a = 0;
  a++;
  int c = 2;
  c *= 2;

  int compteur;

  for (compteur = 0 ; compteur < 10 ; compteur++) {
    printf("Salut les Zeros !\n");
  }

  int n[ 10 ];
  int i,j;

  /* initialize elements of array n to 0 */         
  for ( i = 0; i < 10; i++ ) {
    n[ i ] = n[ i + 1 ];
    n[ i ]++;
  }

  return 0;
}