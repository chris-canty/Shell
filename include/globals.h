#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 


//This holds the struct for Jobs so we can use it in other .c files
struct Job {
	bool isValid;
	int jobNum;
	pid_t pid;
	char * cmds;
};