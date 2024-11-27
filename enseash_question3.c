#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFSIZE 128
#define enseash_prompt "enseash % "
#define enseash_exit_message "Bye bye ...\n"

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    char input[BUFSIZE];

    while(1) {
        ret = read(STDIN_FILENO,input,BUFSIZE);
        input[ret-1] = '\0';

        if((strncmp(input,"exit",5) == 0) || ret == 0){
            write(STDOUT_FILENO,enseash_exit_message,11);
            exit(EXIT_SUCCESS);
        }

        else {
            pid = fork();  

            if(pid == 0) {
                execlp(input,input,NULL);
                exit(EXIT_FAILURE);
            }
            else {
                wait(&status);
                write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));
            }
        }

    }
    return 0;
}