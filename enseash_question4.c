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
#define code_exit_string "[exit:%d] "
#define signal_term_string "[sign:%d] "

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    char input[MAXBUFSIZE];
    char return_code[MAXBUFSIZE];

    write(STDOUT_FILENO,welcome_message,strlen(welcome_message));
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));

    while(1) {
        ret = read(STDIN_FILENO,input,MAXBUFSIZE);
        input[ret-1] = '\0'; // remove the last character (the backspace) and replace it with end of string symbol

        if((strncmp(input,"exit",4) == 0) || ret == 0){ // if ctrl+d is pressed, reading process is stopped and ret is thus 0 (empty string size)
            write(STDOUT_FILENO,enseash_exit_message,11);
            exit(EXIT_SUCCESS);
        }

            pid = fork();  

            if(pid == 0) { // child process executes command then exit
                execlp(input,input,NULL);
                exit(EXIT_FAILURE);
            }
            else { // father process wait for child to finish and return exit or signal code of ended child process
                wait(&status);
                write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));
                if(WIFEXITED(status)) {
                    sprintf(return_code,code_exit_string,WEXITSTATUS(status)); // replace %d by child process exit status
                    write(STDOUT_FILENO,return_code,strlen(return_code));
                }
                else if(WIFSIGNALED(status)) {
                    sprintf(return_code,signal_term_string,WTERMSIG(status)); // replace %d by id of signal who caused end of child
                    write(STDOUT_FILENO,return_code,strlen(return_code));
                }
        }
    }
    return 0;
}