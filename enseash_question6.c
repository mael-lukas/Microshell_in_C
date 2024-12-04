#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define MAXBUFSIZE 128
#define enseash_prompt "enseash % "
#define welcome_message "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define enseash_exit_message "Bye bye ...\n"
#define exit_time_string "[exit:%d|%dms] "
#define signal_time_string "[sign:%d|%dms] "

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    char* argv[MAXBUFSIZE];
    struct timespec startTime, stopTime;
    char input[MAXBUFSIZE];
    char return_code[MAXBUFSIZE];

    write(STDOUT_FILENO,welcome_message,strlen(welcome_message));
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));

    while(1) {
        ret = read(STDIN_FILENO,input,MAXBUFSIZE);
        input[ret-1] = '\0'; // remove the last character (the backspace) and replace it with end of string symbol

        clock_gettime(CLOCK_REALTIME,&startTime);

        if((strncmp(input,"exit",4) == 0) || ret == 0){ // if ctrl+d is pressed, reading process is stopped and ret is thus 0 (empty string size)
            write(STDOUT_FILENO,enseash_exit_message,11);
            exit(EXIT_SUCCESS);
        }

        // split input along the " " to separate command and arguments, put everything in argv
        int i = 0;
        char* strToken = strtok(input," "); // cut the content from input and put it in strToken until a ' ' is met
        while (strToken != NULL && i < MAXBUFSIZE) {
            argv[i] = strToken; // fill argv one string at a time with the command and all the arguments
            strToken = strtok(NULL," "); // NULL to keep on splitting the same string
            i++;
        }
        argv[i] = NULL; // last element for execvp needs to be NULL

        pid = fork();

        if(pid == 0) { // child process executes command then exit
            execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
            exit(1);
        }
        else { // father process wait for child to finish and return exit or signal code of ended child process
            wait(&status);
            clock_gettime(CLOCK_REALTIME,&stopTime);
            int delayInMs = ((stopTime.tv_nsec) - (startTime.tv_nsec)) / 1000000;
            write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));

            if(WIFEXITED(status)) {
                sprintf(return_code,exit_time_string,WEXITSTATUS(status),delayInMs); // replace %d by child process exit status and time taken
                write(STDOUT_FILENO,return_code,strlen(return_code));
            }
            else if(WIFSIGNALED(status)) {
                sprintf(return_code,signal_time_string,WTERMSIG(status),delayInMs); // replace %d by id of signal who caused end of child and time taken
                write(STDOUT_FILENO,return_code,strlen(return_code));
            }
        }
    }
    return 0;
}