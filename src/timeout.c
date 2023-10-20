#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

char *get_path(const char *input){
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
    free(pathvar);
    return NULL;
}

int execute_with_timeout(int timeout_seconds, const char *cmd, char *const cmd_args[]) {
    pid_t pid, wpid;
    int status;

    char *path = get_path(cmd);

    if (path == NULL) {
        fprintf(stderr, "Command '%s' not found in PATH.", cmd);
        return -1;
    }

    if ((pid = fork()) == 0) { // Child process
        execv(path, cmd_args);
        perror("execv");
        exit(EXIT_FAILURE);
    } else {
        int sleep_time = timeout_seconds;

        while (sleep_time > 0) { // Wait for the process to finish or timeout
            wpid = waitpid(pid, &status, WNOHANG);
            if (wpid == pid) {
                free(path); // Free the allocated cmd_path
                return WEXITSTATUS(status);
            }
            sleep_time -= 1; // Delay for 1 second
            sleep(1);
        }
        
        if (kill(pid, SIGTERM) == -1) { // Timeout occurred, send TERM signal to process
            perror("kill");
            free(path); // Free the allocated cmd_path
            return -1;
        }

        if (waitpid(pid, &status, 0) == -1) { // Wait for the process to exit after sending TERM signal
            perror("waitpid");
            free(path); 
            return -1;
        }

        free(path); 

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            return -1;
        }
    }
    free(path); 
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Format: %s [snds] [cmd] [cmd-args]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int timeout_seconds = atoi(argv[1]);
    if (timeout_seconds <= 0) {
        fprintf(stderr, "Timeout value must be a positive number.\n");
        return EXIT_FAILURE;
    }

    char *cmd = argv[2]; // Create an array of command arguments for execvp
    char **cmd_args = &argv[2];

    int exit_status = execute_with_timeout(timeout_seconds, cmd, cmd_args);

    if (exit_status != -1) {
        printf("Command exited with status: %d\n", exit_status);
        return exit_status;
    } else {
        fprintf(stderr, "Timeout occurred, command terminated.\n");
        return EXIT_FAILURE;
    }
}
