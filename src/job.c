#include <sys/wait.h>
#include "globals.h"


void checkBackground(struct Job jobs[10]){
    for (int i = 0; i < 10; ++i){
        //printf("%d\n",jobs[i].pid);
        if (jobs[i].isValid == false){
            //Holds the current jobs status
            int status;
            waitpid(jobs[i].pid,&status,WNOHANG);

            //If the background job has completed, clear the object and set isValid to true so another job can take up the spot
            if (WIFEXITED(status) == true){
                printf("[%d] + done [%s]\n",jobs[i].jobNum,jobs[i].cmds);
                jobs[i].isValid = true;
                free(jobs[i].cmds);
                jobs[i].cmds = (char *)malloc(1);
                jobs[i].cmds[0] = '\0';
            }
        }
    }
}