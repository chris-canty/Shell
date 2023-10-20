#pragma once

#include <stdlib.h>
#include "globals.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void runPiping(struct Job jobs[], char * argv[], size_t size);
void externalCommandExc(struct Job jobs[], char * input, char * argv[], size_t size, bool isBackground);