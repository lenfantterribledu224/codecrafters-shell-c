#ifndef PIPELINE_H
#define PIPELINE_H

void do_pipeline(char *args[], int pipe_pos);
int is_builtin(char *cmd);
void run_builtin(char **args, int nargs);
int count_args(char **args);

#endif // PIPELINE_H