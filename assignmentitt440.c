#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define NUM_LENGTH 4
#define NUM_WAIT 2

void getNUM(char num[NUM_LENGTH + 1]) {
  srand(getpid() + getppid());

  num[0] = 49 + rand() % 7;

  for(int i = 1; i < NUM_LENGTH; i++) {
    num[i] = 48 + rand() % 7;
  }
  num[NUM_LENGTH] = '\0';
}

void sigint_handler(int sig);
int main()
 {
   signal(SIGINT, sigint_handler);
    
   while(1) {
    int pipefds[2];
    char num[NUM_LENGTH + 1];
    char buffer[NUM_LENGTH + 1];


    pipe(pipefds);

    pid_t pid = fork();

    if(signal(SIGINT, sigint_handler) ==  SIG_ERR) {
     perror("signal");
     exit(1);
    }

    if(pid == 0) {
      getNUM(num);
      close(pipefds[0]);
      write(pipefds[1], num, NUM_LENGTH + 1);

      printf("Sending number from child to parent... \n");

      sleep(NUM_WAIT);

      exit(EXIT_SUCCESS);
    }

    if(pid > 0) {
      wait(NULL);

      close(pipefds[1]);
      read(pipefds[0], buffer, NUM_LENGTH + 1);
      close(pipefds[0]);
      printf("Parent received: '%s' \n\n", buffer);
    }
  }
  return EXIT_SUCCESS;
}

    void sigint_handler(int sig){

     printf("Message interrupted!\n");
     exit(1);
  } 

