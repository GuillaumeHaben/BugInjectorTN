#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>

int parseur(char *str1, char **str2[], char* symb, int tablib[]);
void double_cara(int* k,int* i,int* j);
void simple_cara(int* k,int* i,int* j);
void freeARG(char **str[], int N,int tablib[]);
void getPos(char pos[500]);
void printPrompt();


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
  int d = k(c,5);
}

int i(int b, char* a){
  printf("cette fonction n'est jamais appelee");
  return 0;
}

int j(char a){
  printf("un caractere : %c",a);
  char b = a+1;
  printf("ce caractere + 1 : %c",b);
  h(b,2);
  return b;
}

int k(int b, char* a){
  printf("cette fonction est appelee, mais elle sert a rien");
  int c = b/2;
  return c+1;
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

  //-------------------------------------------------------

  int N;
  char **arg[50];
  char sca[500];
  int tablib[50];

  //char *multiarg[50][50] = {{"echo","foo",NULL},{"echo","bar",NULL},{"ls",NULL}};
  char symb[49]={';',';'};
  //int N[3] = {3,3,2};
  //int I = 3;

  int continuer = 1;

  while (continuer){
    if(sca[0]!='\0'){
      printPrompt();
    }
    int i;
    sca[0]='\0';
    fgets(sca, 500, stdin);

    if(sca[0]=='\0'){
      break;
    }

    //printf("\n");

    N = parseur(sca, arg, symb, tablib);  //N=taille de tablib=nb de commandes
    //printf("%s\n",arg[0][0]);
    //printf("%s\n",arg[0][1]);
    //printf("%s\n",arg[1][0]);
    //printf("%s\n",arg[1][1]);

    int passer = 0;

    for(i=0;i<N;i++){
      if(sca[0]=='\0'){
        continuer=0;
        break;
      }else if(passer == 1){
        passer=0;
      }else if((arg[i][0][0]=='q' && arg[i][0][1]=='u' && arg[i][0][2]=='i' && arg[i][0][3]=='t') || (arg[i][0][0]=='e' && arg[i][0][1]=='x' && arg[i][0][2]=='i' && arg[i][0][3]=='t')){
        continuer=0;
      }else if(arg[i][0][0]=='h' && arg[i][0][1]=='e' && arg[i][0][2]=='l' && arg[i][0][3]=='p'){
        printf("---------------HELP---------------\n\nquit ..... quitte l'application\nexit ..... quitte l'application\nhelp ..... affiche la page d'aide\n\n---------------HELP---------------\n");
      }else if(arg[i][0][0]=='c' && arg[i][0][1]=='d' && arg[i][0][2]=='\0'){
        if(arg[i][1]!=NULL){
          chdir(arg[i][1]);
        }else{
          printf("veillez indiquer un dossier\n");
        }
      }else if(i<N-1 && symb[i]=='|'){
        passer=1;
        pid_t pid;
        pid = fork();
        if(pid==0){
          int fd[2];
          pipe(fd);
          pid_t pid_2 = fork();
          if(pid_2==0){
            close(fd[0]);
            dup2(fd[1],1);
            execvp(arg[i][0], arg[i]);
            close(fd[1]);
            exit(0);
          }else{
            close(fd[1]);
            dup2(fd[0],0);
            execvp(arg[i+1][0], arg[i+1]);
            close(fd[0]);
            waitpid(pid_2,NULL,0);
            exit(0);
          }
        }else{
          waitpid(pid,NULL,0);
        }
      }else if(i<N-1 && symb[i]=='<'){
        passer = 1;
        if(fork()==0){
          int fd=open(arg[i+1][0], O_RDONLY, 0);
          dup2(fd,0);
          close(fd);
          execvp(arg[i][0], arg[i]);
        }else{
          wait(NULL);
        }
      }else if(i<N-1 && symb[i]=='>'){
        passer = 1;
        if(fork()==0){
          int fd=open(arg[i+1][0], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
          dup2(fd,1);
          execvp(arg[i][0], arg[i]);
          close(fd);
        }else{
          wait(NULL);
        }
      }else if(i<N-1 && symb[i]=='s'){
      passer = 1;
        if(fork()==0){
          int fd=open(arg[i+1][0], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
          dup2(fd,1);
          execvp(arg[i][0], arg[i]);
          close(fd);
        }else{
          wait(NULL);
        }
      }else{
        pid_t pid;
        pid = fork();
        if(pid==0){
          //printf("1er : %s 2eme : %s\n", arg[i][0], arg[i][1]);
          execvp(arg[i][0], arg[i]);
          exit(0);
        }else{
          wait(NULL);
        }
      }
    }
    freeARG(arg,N,tablib);
  }

  //------------------------------


  f(7);
  g(age);
  h(5,age);

  int age2 = age+1;
  int age3 = age2/2;
  int age4 = age3+10;
  int age5 = age4*2;
  int age6 = age5/3;

  h(age6,5);

  return 0;
}

int parseur(char *str1, char **str2[], char* symb, int tablib[]){
  if(str1[0]!='\0'){
    char a[50];
    int l = strlen(str1)+1;
    int N = 0;
    int i;
    int j=0;
    int k=0;
    int compteursymb=0; //nombre de symboles séparateurs de commandes
    str2[0]=(char**)malloc(10*sizeof(char*));
    for(i=0;i<l;i++){
      a[j]=str1[i];
      //printf("i=%d\n",i);
      if((i+1<l) && (str1[i]=='|') && (str1[i+1]=='|')){
        symb[compteursymb]='o';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        double_cara(&k,&i,&j);
      }
      else if((i+1<l) && (str1[i]=='&') && (str1[i+1]=='&')){
        //printf("oui\n");
        symb[compteursymb]='&';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        double_cara(&k,&i,&j);
      }
      else if((i+1<l) && (str1[i]=='<') && (str1[i+1]=='<')){
        symb[compteursymb]='i';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        double_cara(&k,&i,&j);
      }
      else if((i+1<l) && (str1[i]=='>') && (str1[i+1]=='>')){
        symb[compteursymb]='s';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        double_cara(&k,&i,&j);
      }

      else if (str1[i]=='|'){
        symb[compteursymb]='|';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        simple_cara(&k,&i,&j);
      }
      else if (str1[i]==';'){
        symb[compteursymb]=';';
        //str2[compteursymb][k]=(char*)malloc(sizeof(char));
        //str2[compteursymb][k]=NULL;
        //tablib[compteursymb]=k+1;
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        simple_cara(&k,&i,&j);
      }
      else if (str1[i]=='<'){
        symb[compteursymb]='<';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        simple_cara(&k,&i,&j);
      }
      else if (str1[i]=='>'){
        symb[compteursymb]='>';
        compteursymb++;
        str2[compteursymb]=(char**)malloc(10*sizeof(char*));
        simple_cara(&k,&i,&j);
      }

      else if(str1[i]==' ' || str1[i]=='\0' || str1[i]=='\n' || str1[i]==(char)-1){
        N++;
        a[j]='\0';
        str2[compteursymb][k]=(char *)malloc(j*sizeof(char)+1);
        strcpy(str2[compteursymb][k],a);
        //printf("%s\n",str2[compteursymb][k]);
        k++;
        tablib[compteursymb]=k;
        j=0;
      }else{
        j++;
      }
    }
    str2[compteursymb][k]=(char*)malloc(sizeof(char));
    str2[compteursymb][k-1]=NULL;
    tablib[compteursymb]=k+1;
    return (compteursymb+1);    //on retourne la taille de tablib pour le free
  }else{
    return 0;
  }
}

void double_cara(int* k,int* i,int* j){
  *k=0;
  *j=0;
  ++(*i);       //on ne regarde pas le 2eme caractere du symbole
  ++(*i);       //ni l'espace qui suit
}

void simple_cara(int* k,int* i,int* j){
  *k=0;
  *j=0;
  ++(*i);    //on ne regarde pas l'espace qui suit le symbole
}

void freeARG(char **str[], int N, int tablib[]){
  int j;
  int i;
  int K;

  for(j=0;j<N;j++){
    K=tablib[j];
    for(i=0;i<K;i++){
      free(str[j][i]);
    }
    free(str[j]);
  }
}

int multiCommande(char *str[],char symb[], int Ncom[]){
  return 0;
}

void getPos(char pos[500]){
  int fd[2];
  pipe(fd);
  int i = 0;
  char buffer;

  if(fork()==0){
    close(fd[0]);
    dup2(fd[1],1);
    execvp("pwd",(char *[]){"pwd",NULL});
    close(fd[1]);
  }else{
    close(fd[1]);
    while(read(fd[0],&buffer,1)>0){
      if(buffer != '\n'){
        pos[i]=buffer;
        i++;
      }else{
        pos[i]='\0';
        break;
      }
    }
    wait(NULL);
  }
}

void printPrompt(){
  char *user;
  char host[100];
  char pos[500];
  user = getenv("USER");
  gethostname(host,100*sizeof(char));
  getPos(pos);
  printf("%s@%s:%s$ ", user, host, pos);
}
