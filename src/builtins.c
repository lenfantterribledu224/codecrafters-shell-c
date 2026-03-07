#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>

void do_echo(char *args) {
    char *p = args;
    char output[1024];
    int i = 0;
    int in_space = 0;

    while (*p != '\0' && i < (int)sizeof(output) - 1) {
        if (*p == '\'') {
            in_space = 0;
            p++;
            while (*p != '\'' && *p != '\0') {
                output[i++] = *p++;
            }
            if (*p == '\'') p++;
        } else if (isspace(*p)) {
            if (i > 0 && !in_space) {
                output[i++] = ' ';
                in_space = 1;
            }
            p++;
        } else {
            in_space = 0;
            output[i++] = *p++;
        }
    }

    if (i > 0 && output[i - 1] == ' ') i--;
    output[i] = '\0';
    printf("%s\n", output);
};

void do_exit(char *args) {
    exit(0);
}
Builtin builtins[] = {
    {"echo", do_echo},
    {"exit", do_exit},
    {"type", do_type},
    {"pwd", do_pwd},
    {"cd", do_cd}
};
int num_builtins = sizeof(builtins) / sizeof(builtins[0]);

void do_type(char *args) {
    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(args, builtins[i].name) == 0) {
            printf("%s is a shell builtin\n", args);
            return;
        }
    }
    // not a builtin, search PATH
    char *path = getenv("PATH");
    if (path == NULL) {
        printf("%s: not found\n", args);
        return;
    }
    char path_copy[1024];
    strcpy(path_copy, path);

    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, args);
        if (access(full_path, X_OK) == 0) {
            printf("%s is %s\n", args, full_path);
            return;
        }

        dir = strtok(NULL, ":");
    }
    printf("%s: not found\n", args);
}

void do_execute(char *command) {
    // split command into tokens
    char *args[1024];
    int i = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // execvp needs NULL at the end
     pid_t pid = fork();
     if (pid == 0) {
        execvp(args[0], args);
        printf("%s: not found\n", args[0]);
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
};

void do_pwd(char *args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
};

void do_cd(char *args) {
    if (args == NULL || strcmp(args, "~") == 0) {
        args = getenv("HOME");
    }
    if (chdir(args) != 0) {
        printf("cd: %s: No such file or directory\n", args);
    }
};
