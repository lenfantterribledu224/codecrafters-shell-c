#ifndef SHELL_H
#define SHELL_H

typedef struct {
    char *name;
    void (*func)(char *args);
} Builtin;


void do_echo(char *args[], int nargs);
void do_exit(char *args);
void do_type(char *args[], int nargs);
void do_execute(char *args[], int nargs);

void do_pwd(char *args[], int nargs);
void do_cd(char *args[], int nargs);



extern Builtin builtins[];
extern int num_builtins;

#endif