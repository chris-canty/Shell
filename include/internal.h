#pragma once

#include "globals.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void goToHome();
void changeDirectory(char * path);

bool exitShell(struct Job jobs[10]);
bool getJobs(struct Job jobs[10], bool print);