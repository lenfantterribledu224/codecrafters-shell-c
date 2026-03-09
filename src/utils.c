#include <stdlib.h>
#include <ctype.h>
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
            } else {
                token[i++] = *p++;
            }
        }

        token[i] = '\0';
        args[(*argc)++] = token;
    }

    args[*argc] = NULL;
}