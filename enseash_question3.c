#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXBUFSIZE 128
#define enseash_prompt "enseash % "
#define welcome_message "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define enseash_exit_message "Bye bye ...\n"

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    char input[MAXBUFSIZE];

    write(STDOUT_FILENO,welcome_message,strlen(welcome_message));
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));

    while(1) {
        ret = read(STDIN_FILENO,input,MAXBUFSIZE);
        input[ret-1] = '\0'; // remove the last character (the backspace) and replace it with end of string symbol

        if((strncmp(input,"exit",4) == 0) || ret == 0){ // if ctrl+d is pressed, reading process is stopped and ret is thus 0 (empty string size)
            write(STDOUT_FILENO,enseash_exit_message,strlen(enseash_exit_message));
            exit(EXIT_SUCCESS);
        }

        else {
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

    }
    return 0;
}