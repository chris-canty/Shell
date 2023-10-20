#include "internal.h"
#include "globals.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/wait.h>


void goToHome(){
    chdir(getenv("HOME"));
}

void changeDirectory(char * path){
    struct stat stats;
    stat(path, &stats);

    if(access(path,F_OK) == -1){
        printf("%s: Target does not exist.\n", path);
    }
    else if(S_ISDIR(stats.st_mode) == 0){
        printf("%s: Target not a directory.\n", path);
    }
    else if(chdir(path) == 0){
        //path changes
    }
    else{
        printf("%s: No such file or directory.\n", path);
    }
}

bool exitShell(struct Job jobs[10]){
    if(getJobs(jobs,false) == true){
        printf("Unable to exit. Background processes still running.\n");
        return false;
    }else{
        return true;
    }
}

bool getJobs(struct Job jobs[10],bool print){
    bool busy = false;
    for(int i = 0; i < 10; i++){
        if (jobs[i].isValid == false){
            int status;
            waitpid(jobs[i].pid,&status,WNOHANG);
            if (WIFEXITED(status) == true){
                busy = true;
                if(print == true){
                    printf("[%d] + [%d] [%s]\n",jobs[i].jobNum,jobs[i].pid,jobs[i].cmds);
                }
            }
        }
    }
    if(!busy && print){printf("No active background proccesses.\n");}
    return busy;
}