#include "enseashUtils.h"

int main() {
    write(STDOUT_FILENO,welcome_message,strlen(welcome_message)); // STDOUT_FILENO is default output to write in the shell
    write(STDOUT_FILENO,enseash_prompt,strlen(enseash_prompt));
    return 0;
}