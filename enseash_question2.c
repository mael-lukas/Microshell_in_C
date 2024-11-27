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
    char input[BUFSIZE];
    ret = read(STDIN_FILENO,input,BUFSIZE);
    input[ret-1] = '\0';

    pid_t pid;
    pid = fork();

    if(pid == 0) {
        execl(input,input,NULL);
        exit(EXIT_FAILURE);
    }
    else {
        wait(&status);
        write(STDOUT_FILENO,enseash_prompt,9);
    }
    return 1;
}