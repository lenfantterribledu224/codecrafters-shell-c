#ifndef SHELL_H
#define SHELL_H

typedef struct {
    char *name;
    void (*func)(char *args);
} Builtin;


void do_echo(char *args);
void do_exit(char *args);
void do_type(char *args);

#endif