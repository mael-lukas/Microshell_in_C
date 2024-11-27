#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXBUFSIZE 128
#define welcome_message "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define enseash_prompt "enseash % "

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    char input[MAXBUFSIZE];

    while(1) {
        ret = read(STDIN_FILENO,input,MAXBUFSIZE);
        input[ret-1] = '\0'; // remove the last character (the backspace) and replace it with end of string symbol

        pid = fork();

        if(pid == 0) { // child process executes command then exit
            execlp(input,input,NULL);
            exit(EXIT_FAILURE);
        }
        else { // father process wait for child to finish and give back control to user
            wait(&status);
            write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));
        }
    }
    return 0;
}