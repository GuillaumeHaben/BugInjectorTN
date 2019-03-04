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

int sum(int num1, int num2) {
   int num3;
   num3 = num1 + num2;
   return (num3);
}

int mult(int num1, int num2) {
   int num3;
   num3 = num1 * num2;
   return (num3);
}

int
main(void)
{
	int five = sum(2,3);
	if (1==1) {
		if (2==2) {
			mult(sum(2,3),five);
		}
	}
	return 0;
}

