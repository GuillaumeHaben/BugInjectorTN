#include <stdio.h>
#include <stdlib.h>
/*
int main(void) {
  int age;
  int coucou1;
  int coucou2;
  int prof2;

  puts("Donnez votre age :");
  scanf("%d", &age);
  scanf("%d %d", &coucou1, &coucou2);
  printf("Vous avez %d ans !\n", age);
  prof2=age+1;
  return 0;
}

*/int main2(void) {
  int age2;
  int age3;
  int coucou12;
  int coucou22;
  FILE* fichier = NULL;

  fichier = fopen("test.txt", "r+");

  puts("Donnez votre age :");
  scanf("%d", &age2);
  scanf("%d", &age3);
  scanf("%d %d", &coucou12, &coucou22);
  printf("Vous avez %d ans !\n", age2);
  return 0;
}
