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
    } else if (strcmp(args[0], "cd") == 0) {
        do_cd(args, nargs);
    }
}

void do_pipeline(char *args[], int pipe_pos) {
    // Split into two commands
    args[pipe_pos] = NULL;
    char **left_cmd = args;
    char **right_cmd = &args[pipe_pos + 1];

    // Create the pipe
    int fd[2];
    pipe(fd);

    // Fork Child 1 (left command — writer)
    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        if(is_builtin(left_cmd[0])) {
            run_builtin(left_cmd, count_args(left_cmd));
            exit(0);
        }
        execvp(left_cmd[0], left_cmd);
        perror("execvp");
        exit(1);
    }

    // Fork Child 2 (right command — reader)
    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        if(is_builtin(right_cmd[0])) {
            run_builtin(right_cmd, count_args(right_cmd));
            exit(0);
        }else {
        execvp(right_cmd[0], right_cmd);
        perror("execvp");
        exit(1);
        }

    }

    // Parent: close both pipe ends, wait for both children
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}