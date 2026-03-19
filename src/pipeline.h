#include <sys/types.h>
#ifndef PIPELINE_H
#define PIPELINE_H

void do_pipeline(char *args[], int nargs);
int is_builtin(char *cmd);
void run_builtin(char **args, int nargs);
int count_args(char **args);
int split_on_pipes(char *args[], int nargs, char **cmds[]);
void create_pipes(int pipes[][2], int num_pipes);
void fork_children(char **cmds[], int num_cmds, int pipes[][2], pid_t pids[]);

#endif // PIPELINE_H