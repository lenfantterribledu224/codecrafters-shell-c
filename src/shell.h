#ifndef SHELL_H
#define SHELL_H
#include <sys/wait.h>

typedef struct {
    char *name;
    void (*func)(char *args[], int nargs);
} Builtin;

typedef struct {
    int job_number;
    pid_t pid;
    char command[1024];
    char status[16];
} Job;

void do_echo(char *args[], int nargs);
void do_exit(char *args[], int nargs);
void do_type(char *args[], int nargs);
void do_execute(char *args[], int nargs);
void do_jobs(char *args[], int nargs);
void do_pwd(char *args[], int nargs);
void do_cd(char *args[], int nargs);
void do_history(char *args[], int count, int limit);
void do_background(char *args[], int nargs);
void reap_jobs();

extern int job_count;
extern Job jobs[256];
extern Builtin builtins[];
extern int num_builtins;

#endif