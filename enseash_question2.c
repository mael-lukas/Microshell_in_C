#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFSIZE 128
#define enseash_prompt "enseash % "

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    char input[BUFSIZE];
    while(1) {
         ret = read(STDIN_FILENO,input,BUFSIZE);
        input[ret-1] = '\0';

        pid = fork();  

        if(pid == 0) {
            execl(input,input,NULL);
            exit(EXIT_FAILURE);
        }
        else {
            wait(&status);
            write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));
        }
    }
    return 0;
}