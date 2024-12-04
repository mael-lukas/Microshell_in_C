#include "enseashUtils.h"

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