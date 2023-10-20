#pragma once

#include <stdlib.h>
#include <stdbool.h>

char * get_environment(char * input);
char * tilde_exp(char * input);
char * get_path(char * input);
char * path_to_cmd(char * input);