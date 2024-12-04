#include "enseashUtils.h"

// father process wait for child to finish and return exit code of child process or code of termination signal
void fatherProcessRoutine1(int status) {
    char return_code[MAXBUFSIZE];

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

// adds the return time taken by process to the routine
void fatherProcessRoutine2(int status, struct timespec startTime, struct timespec stopTime) {
    char return_code[MAXBUFSIZE];

    wait(&status);
    clock_gettime(CLOCK_REALTIME,&stopTime);
    int delayInMs = ((stopTime.tv_nsec) - (startTime.tv_nsec)) / 1000000; // get elapsed time in ms
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));

    if(WIFEXITED(status)) {
        sprintf(return_code,exit_time_string,WEXITSTATUS(status),delayInMs); // replace %d by child process exit status and time taken
        write(STDOUT_FILENO,return_code,strlen(return_code));
    }
    else if(WIFSIGNALED(status)) {
        sprintf(return_code,signal_time_string,WTERMSIG(status),delayInMs); // replace %d by id of termination signal and time taken
        write(STDOUT_FILENO,return_code,strlen(return_code));
    }
}

// split input along the " " to separate command and arguments, put everything in output
void splitInput(char* input, char** output) {
    int i = 0;
    char* strToken = strtok(input," "); // cut the content from input and put it in strToken until a " " is met
    while (strToken != NULL && i < MAXBUFSIZE) {
        output[i] = strToken; // fill output one string at a time with the command and all the arguments
        strToken = strtok(NULL," "); // NULL to keep on splitting the same string
        i++;
    }
    output[i] = NULL; // last element set to NULL because output will be used for execvp function
}
