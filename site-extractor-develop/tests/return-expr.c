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
#include <string.h>

int main(void)
{
  return 0;
}
char sampleFunctionChar ()
{
  char c = 'A';
  if (c == 'A') return c;
  return 'A';
}

float sampleFunctionFloat ()
{
  float f = 1.2;
  if (f == 1.2) return f;
  return 1.2;
}

int sampleFunctionInteger ()
{
  int i = 1;
  if (i == 1) return i-3;
  return 1;
}

char * sampleFunctionString()
{
  char * str = "My String";
  if (strcmp(str,"My String")) return str;
  return "My String";
}

typedef int testType;
testType sampleFunctionTypedef()
{
  testType t = 1;
  if (t == 1) return t;
  return 1;
}

typedef struct { int a; int b; } bah;
bah sampleFunctionCompoundLiteralExpr()
{
  bah a = { 1,2 };
  if (a.a == 1 && a.b == 2) return a;
  return (bah){1, 2};
}