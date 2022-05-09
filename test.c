#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>


int fonction();


void handler(int theSignal)
{

  pid_t pid;

  printf("\n");
  
  pid = fork();

  if(!pid)
  {
    printf("child = %d\n", pid);
    printf("4\n");
    fonction();
    printf("5\n");
  }

  if(pid)
  { 
    printf("4 bis\n");
    printf("parent = %d\n", pid);
    wait(0);
    exit(0);
  }

}


int fonction()
{ 
  char *str;

  printf("1\n");
  
  struct sigaction prepaSignal;

  printf("2\n");
  
  prepaSignal.sa_handler=&handler;

  printf("3\n");
  
  sigaction(SIGINT,&prepaSignal,0);

  printf("6\n");
  
  
  while (1)
  {
    ;
  }

  printf("7\n");

  return 1;
}




int main(void)
{
  fonction();

  return (1);
}