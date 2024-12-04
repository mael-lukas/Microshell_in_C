//
// Created by administrateur on 04/12/24.
//

#ifndef ENSEASHUTILS_H
#define ENSEASHUTILS_H

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

#define MAXBUFSIZE 128
#define enseash_prompt "enseash % "
#define welcome_message "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n"
#define enseash_exit_message "Bye bye ...\n"
#define code_exit_string "[exit:%d] "
#define signal_term_string "[sign:%d] "
#define exit_time_string "[exit:%d|%dms] "
#define signal_time_string "[sign:%d|%dms] "
#define NO_REDIRECTION 0
#define REDIRECT_OUTPUT 1
#define REDIRECT_INPUT 2
#define USR_GRP_OTH_PERM 0644

void fatherProcessRoutine1(int status);

void fatherProcessRoutine2(int status, struct timespec startTime, struct timespec stopTime);

void splitInput(char* input, char** output);

int setupRedirection(char** argv, int* redirectionIndex);

#endif //ENSEASHUTILS_H
