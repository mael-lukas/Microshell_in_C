#include "enseashUtils.h"

int main() {
    int status;
    ssize_t ret;
    pid_t pid;
    struct timespec startTime, stopTime;
    int redirectionIndex = 0;
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
        int redirectionType = setupRedirection(argv,&redirectionIndex);

        pid = fork();
        if (pid == 0) {
            if (redirectionType == NO_REDIRECTION) {
                execvp(argv[0],argv);
                exit(EXIT_SUCCESS);
            }

            if (redirectionType == REDIRECT_INPUT) {
                int fd = open(argv[redirectionIndex + 1],O_RDONLY);
                if (fd == -1) {
                    perror("Could not open the file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd,STDIN_FILENO); // changes shell input (STDIN) for file input
                close(fd);

                execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
                exit(EXIT_SUCCESS);
            }

            if (redirectionType == REDIRECT_OUTPUT) {
                int fd = open(argv[redirectionIndex + 1],O_CREAT | O_WRONLY | O_TRUNC, USR_GRP_OTH_PERM);
                if (fd == -1) {
                    perror("Could not open the file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd,STDOUT_FILENO); // changes shell output (STDOUT) for file output
                close(fd);

                execvp(argv[0],argv); // v means all arguments are given all at once with a char** (here argv) with last element as NULL
                exit(EXIT_SUCCESS);
            }
        }
        else {
            fatherProcessRoutine2(status,startTime,stopTime);
        }
        redirectionType = NO_REDIRECTION;
    }
    return 0;
}