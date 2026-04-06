#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "utils.h"

int count_args(char **args) {
    int n = 0;
    while (args[n] != NULL) n++;
    return n;
}
int is_builtin(char *cmd) {
    return (strcmp(cmd, "echo") == 0 ||
            strcmp(cmd, "type") == 0 ||
            strcmp(cmd, "pwd") == 0 ||
            strcmp(cmd, "cd") == 0 ||
            strcmp(cmd, "exit") == 0);
}

void run_builtin(char **args, int nargs) {
    if (strcmp(args[0], "echo") == 0) {
        do_echo(args, nargs);
    } else if (strcmp(args[0], "type") == 0) {
        do_type(args, nargs);
    } else if (strcmp(args[0], "pwd") == 0) {
        do_pwd(args, nargs);
    } else if(strcmp(args[0], "jobs") ==0){
        do_jobs(args, nargs);
    }
    else if (strcmp(args[0], "cd") == 0) {
        do_cd(args, nargs);
    }
}

int split_on_pipes(char *args[], int nargs, char **cmds[]) {
    int num_cmds = 0;

    cmds[num_cmds++] = args;
    for (int i = 0; i < nargs; i++) {
        if (strcmp(args[i], "|") == 0) {
            free(args[i]);
            args[i] = NULL;
            cmds[num_cmds++] = &args[i + 1];
        }
    }
    return num_cmds;
};
void create_pipes(int pipes[][2], int num_pipes) {
    for (int i = 0; i < num_pipes; i++) {
        pipe(pipes[i]);
    }
}

void fork_children(char **cmds[], int num_cmds, int pipes[][2], pid_t pids[]) {
    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            // close all pipe fds
            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            // after closing all pipe fds
            if (is_builtin(cmds[i][0])) {
                run_builtin(cmds[i], count_args(cmds[i]));
                exit(0);
            } else {
                execvp(cmds[i][0], cmds[i]);
                perror("execvp");
                exit(1);
            }
        }
    }
}
void do_pipeline(char *args[], int nargs) {
    // Split into  commands
     char **cmds[64];
    int num_cmds = split_on_pipes(args, nargs, cmds);
    int pipes[num_cmds - 1][2];
    // Create the pipe
    create_pipes(pipes, num_cmds - 1);

    pid_t pids[64];
    fork_children(cmds, num_cmds, pipes, pids);

     // Parent: close all pipe fds
    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_cmds; i++) {
        waitpid(pids[i], NULL, 0);
    }
}
