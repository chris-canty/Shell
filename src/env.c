#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 


//Input Example: $HOME char * input = ['$','H','O','M','E','\0']
char * get_environment(char * input){
	//Allocating Envar to get rid of that $ ($PATH -> PATH)
	char * envar = (char *)malloc(strlen(input));
	char * path;

	//Copying over env variable (Ex: $HOME -> HOME)
	if (strlen(input) > 1) {
        strcpy(envar, input + 1);
	}
	if (getenv(envar) == NULL){
		path = malloc(strlen("null") + 1);
		strncpy(path, "null", strlen("null") + 1);
	}
	else{
		envar = getenv(envar); //envar = getenv("HOME");
		path = (char *)malloc(strlen(envar));
		//Calling the PATH and making a deep copy before returning
		strcpy(path,envar);
	}

	
	return path;
}


char * tilde_exp(char * input){
	char * tildeSubDirectory; 
	if(input[1]=='/'){
		tildeSubDirectory = malloc(strlen(input));
		strcpy(tildeSubDirectory, input + 1);

		//Need to allocate memory accounting for the null terminator
		char *expandedPath = malloc(strlen(getenv("HOME")) + strlen(tildeSubDirectory) + 1);
		strcpy(expandedPath, getenv("HOME"));
		strcat(expandedPath, tildeSubDirectory);

		//Replace with expanded path
		free(input);
		input = expandedPath;

		//Free tildeSubDirectory memory
		free(tildeSubDirectory);
	}
	else{
		strcpy(input, getenv("HOME"));
	}
	return input;
}

char * get_path(char * input){
	//Variables for holding temp strings
	char * pathvar;
	char * token;
	char * filepath;

	//Allocating enought space to hold the PATH
	pathvar =  malloc(strlen(getenv("PATH")) + 1);

	strcpy(pathvar, getenv("PATH"));


	//In this loop, we are iterating though each filepath, the PATH variable seperates the paths by a ':', we iterate until we don't have any paths left
	token = strtok(pathvar, ":");
	while (token != NULL) {
        filepath = malloc(strlen(input) + strlen(token) + 2);
		strcat(filepath,token);
		strcat(filepath,"/");
		strcat(filepath,input);

		//ex: filepath = /usr/bin/ls if we typed in ls, access() checks whether the file exists in that filepath
		if (access(filepath, F_OK) != -1) {
        	return filepath;
		}

		//We keep it NULL to tell strtok that we are continuing the same string we had before
		token = strtok(NULL, ":");
	}

	//This basically just returns an error message saying there was not a valid path
	char * notfound = malloc(21 + strlen(input));
	strcat(notfound,input);
	strcat(notfound,": Command not found.");
	return notfound;
}


//We use this function to translate a path back into a command input
char * path_to_cmd(char * input){
	char * cmd = strrchr(input, '/');
	cmd++;
	return cmd;
}