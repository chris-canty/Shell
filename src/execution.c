#include "execution.h"
#include "env.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/wait.h> 

//A global variable to keep track of background jobs
int jobNumber = 0;


void runPiping(struct Job jobs[10],char * argv[], size_t size){
    //Array Of commands
    char *** cmds = malloc(sizeof(char **));

    //Temp list of args
    char ** args = malloc(sizeof(char *));

    //Size of each args
    size_t * argSize = malloc(sizeof(size_t));

    //Is it a Background Process
    bool isBackground = false;

    //Current Command
    int currCMD = 0;


    argSize[0] = 0;
    
    for (int i = 0; i < size; ++i){
        //Once we find a pipe we create a new array of commands and store the old one in cmds array
        if (strcmp(argv[i],"|") == 0){
            cmds[currCMD] = args;
            args = malloc(sizeof(char *));
            currCMD++;
            int newSize = currCMD + 1;
            cmds = realloc(cmds,newSize * sizeof(char **));
            argSize = realloc(argSize,sizeof(size_t) * newSize);
            argSize[currCMD] = 0;
        }

        //Get each input, increase the size of arguments, and place it in the args array
        else{
            if (strcmp(argv[i],"&") == 0){
                isBackground = true;
            }
            else{
                args[argSize[currCMD]] = (char *)malloc(strlen(argv[i]) + 1);
                strcpy(args[argSize[currCMD]],argv[i]);
                argSize[currCMD] = argSize[currCMD] + 1;
                args = realloc(args,argSize[currCMD] * sizeof(char *) + 1);
            }
        }  
    }
    cmds[currCMD] = args;
    
   
    //The Execution Piping
    if (currCMD > 0){
        for (int i = 0; i <= currCMD; ++i){
            char * path = get_path(cmds[i][0]);
            if (!(strstr(path, ": Command not found."))){
                cmds[i][0] = realloc(cmds[i][0],strlen(path) + 1);
                strcpy(cmds[i][0],get_path(cmds[i][0]));
            }
            else{
                printf("%s\n",path);
                return;
            }
        }
        int **pipes = malloc((currCMD + 1) * sizeof(int*));
        for (int i = 0; i < currCMD; ++i){
            pipes[i] = malloc(sizeof(int) * 2);
            pipe(pipes[i]);
        }
        pid_t * pid = malloc(sizeof(pid_t *) * currCMD);
        for(int i = 0; i <= currCMD; ++i){
            pid[i] = fork();
            if (pid[i] == 0) {
                if (i == 0){
                dup2(pipes[i][1], STDOUT_FILENO);
                for (int j = 0; j < currCMD; ++j){
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                execv(cmds[i][0],cmds[i]);
                }
                else if (i == currCMD){
                    for (int j = 0; j < currCMD - 1; ++j){
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }
                    close(pipes[i - 1][1]);
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    close(pipes[i - 1][0]);
                    execv(cmds[i][0],cmds[i]);
                }
                else{
                    for (int j = 0; j < currCMD; ++j){
                        //We want to make sure we dont close the read of the last pipe or write of the next pipe until we use dup2()
                        if ((i - 1) == j){
                            close(pipes[j][1]);
                        }
                        else if (i == j){
                            close(pipes[j][0]);
                        }
                        else{
                            close(pipes[j][0]);
                            close(pipes[j][1]);
                        }
                    }
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    dup2(pipes[i][1], STDOUT_FILENO);
                    close(pipes[i - 1][0]);
                    close(pipes[i][1]);
                    execv(cmds[i][0],cmds[i]);
                }
            }
        }
        if (isBackground == true && pid[currCMD] != 0){
            jobNumber++;
            int i = 0;
            while(jobs[i].isValid != true && i < 10){
                ++i;
            }
            if (i == 10){
                return;
            }
            else{
                jobs[i].pid = pid[currCMD];
                jobs[i].jobNum = jobNumber;
                jobs[i].isValid = false;
                jobs[i].cmds = malloc(strlen(jobs[i].cmds) + strlen(argv[0]) + 1);
                for (int c = 0; c < size - 1; ++c){
                    strcat(jobs[i].cmds,argv[c]);
                    jobs[i].cmds = realloc(jobs[i].cmds,strlen(jobs[i].cmds) + (sizeof(char) * 3));
                    strcat(jobs[i].cmds," ");
                    if (c != (size -2 )){
                        jobs[i].cmds = realloc(jobs[i].cmds,strlen(jobs[i].cmds) + strlen(argv[c + 1]) + 1);
                    } 
                }
            }
            printf("[%d] [%d]\n",jobs[i].jobNum,jobs[i].pid);
        }
        for (int i = 0; i < currCMD; ++i){
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(pipes[i]);
        }
        if (isBackground == true){
            for (int i = 0; i <= currCMD; ++i){
                int status;
                waitpid(pid[i],&status,WNOHANG);
            }
        }
        else{
            for (int i = 0; i <= currCMD; ++i){
                int status;
                waitpid(pid[i],&status,0);
            }
        } 
        free(pipes);
        free(pid);
        
    }

    //The execution for no piping, handled by external function.
    else{
        cmds[0][0] = get_path(cmds[0][0]);
        if (!(strstr(cmds[0][0], ": Command not found."))){
            externalCommandExc(jobs, cmds[0][0], cmds[0], argSize[0], isBackground);
        }
        else{
            printf("%s\n",cmds[0][0]);
        }
    }
}

void externalCommandExc(struct Job jobs[10],char * input, char * argv[], size_t size, bool isBackground){
    //Holds the status and pid of the current job
	int status;
	pid_t pid = fork();


    if (isBackground == true && pid != 0){
        jobNumber++;
        int i = 0;
        while(jobs[i].isValid != true && i < 10){
            ++i;
        }
        if (i == 10){
            return;
        }
        else{
            jobs[i].pid = pid;
            jobs[i].jobNum = jobNumber;
            jobs[i].isValid = false;
            char * cmd = path_to_cmd(argv[0]);
            jobs[i].cmds = (char *)malloc(strlen(cmd) + 1);
            strcpy(jobs[i].cmds,cmd);
            for (int j = 1; j < size; ++j){
                jobs[i].cmds = realloc(jobs[i].cmds,sizeof(char) + strlen(jobs[i].cmds) + 1);
                strcat(jobs[i].cmds," ");
                jobs[i].cmds = realloc(jobs[i].cmds,strlen(jobs[i].cmds) + strlen(argv[j]) + 1);
                strcat(jobs[i].cmds,argv[j]);
            }
        }
        printf("[%d] [%d]\n",jobs[i].jobNum,jobs[i].pid);
    }
    //We use these for File I/O
    int outfd = -1;
    int outfile = -1;
    int infd = -1;
    int infile = -1;

    //Used for processing the new args array after checking for file I/O
    char ** args = (char **)malloc(sizeof(char *));
    args[0] = input;
    size_t currentSize = 1;

    

    //Child Process
	if(pid == 0){
        for (int i = 1; i < size; ++i){
            if (strcmp(argv[i],">") == 0){
                //For File Output
                outfd = dup(STDOUT_FILENO);
                close(STDOUT_FILENO);
                outfile = open(argv[i+1],O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                ++i;
            }
            else if (strcmp(argv[i],"<") == 0){
                //For File Input
                infd = dup(STDIN_FILENO);
                close(STDIN_FILENO);
                infile = open(argv[i+1],O_RDWR, S_IRUSR);
                ++i;
                
            }
            else{
                currentSize++;
                args = realloc(args,currentSize);
                args[currentSize - 1] = argv[i];
            }
        }
        currentSize++;
        args = realloc(args,currentSize);
        args[currentSize - 1] = NULL;
		execv(args[0], args);
        //Closes File out
        if (outfd > -1 && outfile > -1){
            dup2(outfd, STDOUT_FILENO);
            close(outfd);
        }

        //Closes File In
        if (infd > -1 && infile > -1){
            dup2(infd, STDIN_FILENO);
            close(infd);
        }
    }
	else{
		//Parent Process
        if (isBackground == true){
            waitpid(pid, &status,WNOHANG);
        }
        else{
		    waitpid(pid, &status,0);
        }
    }
}