#include <stdio.h>
#include <stdlib.h>

int f(int a){
  int age=5;
  return age+a;
}

void g(int b){
  printf("Vous avez %d an(s) !\n", b);
}

void h(int a, int b){
  int c = a + b;
  printf("la variable suivante est user controled : c=%d",c);
}

int i(int b, char* a){
  printf("cette fonction n'est jamais appelee");
  return 0;
}

int j(char a){
  printf("un caractere : %c",a);
  char b = a+1;
  printf("ce caractere + 1 : %c",b);
  return b;
}

int main(void) {
  int age;
  char c;
  int coucou1;
  int coucou2;

  puts("Donnez votre age :");
  scanf("%d", &age);
  scanf("%d %d", &coucou1,&coucou2);

  FILE *fp;
   int c;
  
   fp = fopen("file.txt","r");
   while(1) {
      c = fgetc(fp);
      if( feof(fp) ) { 
         break ;
      }
      printf("%c", c);
      j(c);
   }
   fclose(fp);

  f(7);
  g(age);
  h(5,age);
  return 0;
}
