#include "shell.h"
#include "env.h"
#include "execution.h"
#include "job.h"
#include "globals.h"
#include "internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 


//This Holds Our Active Background Processes
struct Job jobs[10] = {
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""},
    {true, 0, 0, ""}
};


int main()
{
	//Store Variables related to the command prompt
	char * username = getenv("USER");
    char * machine = getenv("MACHINE");
    char * pwd;

	while (1) {
		//Grabs the current PWD
		pwd = getenv("PWD");

		
		printf("%s@%s:%s>", username, machine,pwd);
		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();


		//We check the background to see if there is any background process currently running
		//Located in: job.c
		checkBackground(jobs);
		

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) {
			if (tokens->items[i][0] == '$'){
				tokens->items[i] = get_environment(tokens->items[i]);
			}else if(tokens->items[i][0] == '~'){
				tokens->items[i] = tilde_exp(tokens->items[i]);
			}
		}

		//We call a function that handles external command execution
		//Located in: execution.c
		if(strcmp(tokens->items[0], "cd") == 0){
			if(tokens->size == 1){
				goToHome();
				getcwd(pwd,200);
			}else if(tokens->size >= 3){
				printf("cd: Too many arguements.");
			}else{
				changeDirectory(tokens->items[1]);
				getcwd(pwd,200);
			}
		}
		else if(strcmp(tokens->items[0], "exit") == 0){
			if(exitShell(jobs) == true){
					free(input);
					free_tokens(tokens);
					exit(0);
			}
		}
		else if(strcmp(tokens->items[0], "jobs") == 0){
			getJobs(jobs,true);
		}
		else{
			runPiping(jobs,tokens->items,tokens->size);
		}
		free(input);
		free_tokens(tokens);
	}

	return 0;
}

char *get_input(void) {
	
	char *buffer = NULL;
	int bufsize = 0;
	char line[5];
	bool firstLine = true;
	while (fgets(line, 5, stdin) != NULL)
	{
		//Checks to see if the first line has any valid input
		if (firstLine == true){
			if (strcmp(line, "\n") == 0){
				buffer = (char *)malloc(sizeof(char));
				strcpy(buffer,"\n");
				return buffer;
			}
			firstLine = false;
		}
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;
		buffer = (char *)realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;
		if (newln != NULL)
			break;
	}
	buffer = (char *)realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;
	return buffer;
}

tokenlist *new_tokenlist(void) {
	tokenlist *tokens = (tokenlist *)malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **)malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item) {
	int i = tokens->size;

	tokens->items = (char **)realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *)malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

tokenlist *get_tokens(char *input) {
	char *buf = (char *)malloc(strlen(input) + 1);
	strcpy(buf, input);
	tokenlist *tokens = new_tokenlist();
	char *tok = strtok(buf, " ");
	while (tok != NULL)
	{
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}
	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens) {
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);
	free(tokens->items);
	free(tokens);
}