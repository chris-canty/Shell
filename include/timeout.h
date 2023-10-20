#ifndef MYTIMEOUT_H
#define MYTIMEOUT_H

int execute_with_timeout(int timeout_seconds, const char *cmd, char *const cmd_args[]);
char *get_path(const char *input);

#endif // MYTIMEOUT_H
