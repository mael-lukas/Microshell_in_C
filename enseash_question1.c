#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define welcome_message "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define enseash_prompt "enseash % "

int main() {
    write(STDOUT_FILENO,welcome_message,strlen(welcome_message)); // STDOUT_FILENO is default output to write in the shell
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));
    return 0;
}