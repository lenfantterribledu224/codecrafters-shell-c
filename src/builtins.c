#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"
#include <dirent.h>

void do_echo(char *args[], int nargs) {
    for (int i = 1; i < nargs; i++) {
        printf("%s", args[i]);
        if (i < nargs - 1) printf(" ");
    }
    printf("\n");
}

void do_exit(char *args[], int nargs) {
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

char *find_in_path(const char *cmd) {
    char *path = getenv("PATH");
    if (path == NULL) {
        printf("%s: not found\n", cmd);
        return NULL;
    }

    char path_copy[1024];
    strcpy(path_copy, path);

    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path);
        }
        dir = strtok(NULL, ":");
    }
    return NULL;
}

void do_type(char *args[], int nargs) {
    if (nargs < 2) return;

    char *cmd = args[1];

    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(cmd, builtins[i].name) == 0) {
            printf("%s is a shell builtin\n", cmd);
            return;
        }
    }

    char *full_path = find_in_path(cmd);
    if (full_path != NULL) {
    printf("%s is %s\n", cmd, full_path);
    free(full_path);
    return;
    }
   printf("%s: not found\n", cmd);
}

void do_execute(char *args[], int nargs) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        printf("%s: not found\n", args[0]);
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
}

void do_pwd(char *args[], int nargs) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    }
}

void do_cd(char *args[], int nargs) {
    char *path = (nargs < 2 || strcmp(args[1], "~") == 0) ? getenv("HOME") : args[1];
    if (chdir(path) != 0) {
        printf("cd: %s: No such file or directory\n", path);
    }
}

void find_in_path_prefix(const char *cmd, char *matches[], int *count)  {
    char *path = getenv("PATH");
    if (path == NULL) return NULL;

    char path_copy[1024];
    strcpy(path_copy, path);

    char *dir = strtok(path_copy, ":");
    while (dir != NULL) {
        DIR *d = opendir(dir);
        if (d != NULL) {
            struct dirent *entry;
            while ((entry = readdir(d)) != NULL) {
                if (strncmp(entry->d_name, cmd, strlen(cmd)) == 0) {
                    char full_path[1024];
                    snprintf(full_path, sizeof(full_path), "%s/%s", dir, entry->d_name);
                    if (access(full_path, X_OK) == 0) {
                        closedir(d);
                        matches[(*count)++] = strdup(entry->d_name);
                    }
                }
            }
            closedir(d);
        }
        dir = strtok(NULL, ":");
    }
    return NULL;
}