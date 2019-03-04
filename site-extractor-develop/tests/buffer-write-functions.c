/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
/*#include <stdio.h>

int main()
{
   FILE *fp;
   char str[60];

   fp = fopen("file.txt" , "r");
   if(fp == NULL) 
   {
      perror("Error opening file");
      return(-1);
   }
   if( fgets (str, 60, fp)!=NULL ) 
   {
      puts(str);
   }
   fclose(fp);
   
   return(0);
}*/

#include <wchar.h>

int main ()
{
  wchar_t wcs1[] = L"To be or not to be";
  wchar_t wcs2[40];
  wchar_t wcs3[40];

  wcsncpy ( wcs2, wcs1, 40 );
  wcsncpy ( wcs3, wcs2, 5 );
  wcs3[5] = L'\0';

  wprintf (L"%ls\n%ls\n%ls\n",wcs1,wcs2,wcs3);

  return 0;
}