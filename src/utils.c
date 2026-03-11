#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void parse_single_quotes(char **p, char *token, int *i) {
    (*p)++;  // skip opening quote
    while (**p != '\'' && **p != '\0') {
        token[(*i)++] = **p;
        (*p)++;
    }
    if (**p == '\'') (*p)++;  // skip closing quote
};

void parse_double_quotes(char **p, char *token, int *i) {
    (*p)++;  // skip opening quote
    while (**p != '"' && **p != '\0') {
        if (**p == '\\') {
            (*p)++;  // skip backslash, next character is literal
        }
        token[(*i)++] = **p;
        (*p)++;
    }
    if (**p == '"') (*p)++;  // skip closing quote
}

void parse_args(char *input, char *args[], int *argc) {
    char *p = input;
    *argc = 0;

    while (*p != '\0') {
       // skip spaces between tokens
        while (isspace(*p)) p++;

        if (*p == '\0') break;

        // start a new token
        char *token = malloc(strlen(input) + 1);
        int i = 0;

        while (*p != '\0' && !isspace(*p)) {
            if (*p == '\'') {
                parse_single_quotes(&p, token, &i);
            } else if (*p == '"') {
                parse_double_quotes(&p, token, &i);
            } else if (*p == '\\') {
                p++;  // skip backslash
                token[i++] = *p++;
            } else if (isspace(*p)){
                break;
            }
            else {
                token[i++] = *p++;
            }
        }

        token[i] = '\0';
        args[(*argc)++] = token;
    }

    args[*argc] = NULL;
};


int handle_redirection(char *args[], int *nargs, int *redirected_fd) {
    int open_flags = O_WRONLY | O_CREAT | O_TRUNC;
    for (int i = 0; i < *nargs; i++) {

        if (strcmp(args[i], ">") == 0 || strcmp(args[i], "1>") == 0) {
            *redirected_fd = STDOUT_FILENO;
        } else if (strcmp(args[i], "2>") == 0) {
            *redirected_fd = STDERR_FILENO;
        } else if (strcmp(args[i], ">>") == 0 || strcmp(args[i], "1>>") == 0) {
            *redirected_fd = STDOUT_FILENO;
             open_flags = O_WRONLY | O_CREAT | O_APPEND;
        } else if (strcmp(args[i], "2>>") == 0) {
            *redirected_fd = STDERR_FILENO;
            open_flags = O_WRONLY | O_CREAT | O_APPEND;
        }

        if (*redirected_fd != -1) {
            char *filename = args[i + 1];
            *nargs = i;
            args[i] = NULL;

            int saved_fd = dup(*redirected_fd);
            int fd = open(filename, open_flags, 0644);
            dup2(fd, *redirected_fd);
            close(fd);

            return saved_fd;
        }
    }
    return -1;
};

void free_args(char *args[], int argc) {
    for (int i = 0; i < argc; i++) {
        free(args[i]);
    }
}