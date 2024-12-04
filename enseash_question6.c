#include "enseashUtils.h"

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    struct timespec startTime, stopTime;
    char input[MAXBUFSIZE];
    char* argv[MAXBUFSIZE];

    write(STDOUT_FILENO,welcome_message,strlen(welcome_message));
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));

    while(1) {
        ret = read(STDIN_FILENO,input,MAXBUFSIZE);
        input[ret-1] = '\0'; // remove the last character (the backspace) and replace it with end of string symbol

        clock_gettime(CLOCK_REALTIME,&startTime);

        if((strncmp(input,"exit",4) == 0) || ret == 0){ // if ctrl+d is pressed, reading process is stopped and ret is thus 0 (empty string size)
            write(STDOUT_FILENO,enseash_exit_message,strlen(enseash_exit_message));
            exit(EXIT_SUCCESS);
        }

        splitInput(input,argv);

        pid = fork();

        if(pid == 0) { // child process executes command then exit
            execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
            exit(EXIT_FAILURE);
        }
        else {
            fatherProcessRoutine2(status, startTime, stopTime);
        }
    }
    return 0;
}