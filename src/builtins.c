#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void do_echo(char *args) {
    printf("%s\n", args);
}

void do_exit(char *args) {
    exit(0);
}
Builtin builtins[] = {
    {"echo", do_echo},
    {"exit", do_exit},
    {"type", do_type},
};
int num_builtins = sizeof(builtins) / sizeof(builtins[0]);
void do_type(char *args) {
    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(args, builtins[i].name) == 0) {
            printf("%s is a shell builtin\n", args);
            return;
        }
    }
    printf("%s: not found\n", args);
}



