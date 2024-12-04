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

        if((strncmp(input,"exit",4) == 0) || ret == 0){ // if ctrl+d is pressed, reading process is stopped and ret is thus 0 (empty string size)
            write(STDOUT_FILENO,enseash_exit_message,strlen(enseash_exit_message));
            exit(EXIT_SUCCESS);
        }

            pid = fork();  

            if(pid == 0) { // child process executes command then exit
                execlp(input,input,NULL);
                exit(EXIT_FAILURE);
            }
            else {
                fatherProcessRoutine1(status);
        }
    }
    return 0;
}