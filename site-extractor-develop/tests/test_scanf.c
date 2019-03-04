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

int main(void) {
  int age;
  char c;
  int coucou1;
  int coucou2;

  puts("Donnez votre age :");
  scanf("%d", &age);
  scanf("%d %d", &coucou1,&coucou2);
  f(7);
  g(age);
  h(5,age);
  return 0;
}
