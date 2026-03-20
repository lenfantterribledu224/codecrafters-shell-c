#ifndef SHELL_H
#define SHELL_H

typedef struct {
    char *name;
    void (*func)(char *args[], int nargs);
} Builtin;


void do_echo(char *args[], int nargs);
void do_exit(char *args[], int nargs);
void do_type(char *args[], int nargs);
void do_execute(char *args[], int nargs);

void do_pwd(char *args[], int nargs);
void do_cd(char *args[], int nargs);
void do_history(char *args[], int count, int limit);



extern Builtin builtins[];
extern int num_builtins;

#endif