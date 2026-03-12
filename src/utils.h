#ifndef UTILS_H
#define UTILS_H
void parse_single_quotes(char **p, char *token, int *i);
void parse_double_quotes(char **p, char *token, int *i);
void parse_args(char *input, char *args[], int *argc);
int handle_redirection(char *args[], int *nargs, int *redirected_fd);
void free_args(char *args[], int nargs);
char *find_in_path(const char *cmd);
char *find_in_path_prefix(const char *cmd);
#endif