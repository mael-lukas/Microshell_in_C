#include "enseashUtils.h"

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    int positionOfRedirection = 0;
    int redirectionType = NO_REDIRECTION;
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
            write(STDOUT_FILENO,enseash_exit_message,strlen(enseash_exit_message));
            exit(EXIT_SUCCESS);
        }

        splitInput(input,argv);

        int j = 0;
        while (argv[j] != NULL) {
            if (strcmp(argv[j],"<") == 0) {
                positionOfRedirection = j;
                redirectionType = REDIRECT_INPUT;
            }
            if (strcmp(argv[j],">") == 0) {
                positionOfRedirection = j;
                redirectionType = REDIRECT_OUTPUT;
            }
            j++;
        }

        if (positionOfRedirection != 0) {
            argv[positionOfRedirection] = NULL; // stops the string of arguments before redirection
        }

        if (redirectionType == NO_REDIRECTION) {
            pid = fork();

            if(pid == 0) { // child process executes command then exit
                execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
                exit(EXIT_SUCCESS);
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

        if (redirectionType == REDIRECT_INPUT) {
            pid = fork();

            if(pid == 0) { // child process executes command then exit
                int fd = open(argv[positionOfRedirection + 1],O_RDONLY);
                if (fd == -1) {
                    perror("Could not open the file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd,STDIN_FILENO); // changes shell input (STDIN) for file input
                close(fd);

                execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
                exit(EXIT_SUCCESS);
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
            redirectionType = NO_REDIRECTION;
        }

        if (redirectionType == REDIRECT_OUTPUT) {
            pid = fork();

            if(pid == 0) { // child process executes command then exit
                int fd = open(argv[positionOfRedirection + 1],O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("Could not open the file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd,STDOUT_FILENO); // changes shell output (STDOUT) for file output
                close(fd);

                execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
                exit(EXIT_SUCCESS);
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
            redirectionType = NO_REDIRECTION;
        }

    }
    return 0;
}