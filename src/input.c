#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "shell.h"
#include "input.h"
#include "utils.h"
#include <stdlib.h>

static struct termios orig_termios;

static void enable_raw_mode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
static void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void sort_matches(char *matches[], int count) {
    for (int a = 0; a < count - 1; a++)
        for (int b = a + 1; b < count; b++)
            if (strcmp(matches[a], matches[b]) > 0) {
                char *tmp = matches[a];
                matches[a] = matches[b];
                matches[b] = tmp;
            }
}
static void show_matches(char *matches[], int count, const char *buf) {
    sort_matches(matches, count);
    printf("\n");
    for (int j = 0; j < count; j++) {
        printf("%s", matches[j]);
        if (j < count - 1)
            printf("  ");
    }
    printf("\n$ %s", buf);
}

static void complete_word(char *buf, int *pos, const char *full) {
    int cur = *pos;
    printf("%s ", full + cur);
    strcpy(buf, full);
    strcat(buf, " ");
    *pos = strlen(buf);
}
static int try_complete_builtin(char *buf, int *pos) {
    for (int j = 0; j < num_builtins; j++) {
        if (strncmp(buf, builtins[j].name, *pos) == 0) {
            complete_word(buf, pos, builtins[j].name);
            return 1;
        }
    }
    return 0;
}

static void try_complete_path(char *buf, int *pos, int last_was_tab) {
    char *matches[1024];
    int count = 0;

    find_in_path_prefix(buf, matches, &count);

    if (count == 1) {
        complete_word(buf, pos, matches[0]);
    } else if (count > 1 && last_was_tab) {
        show_matches(matches, count, buf);
    } else {
        printf("\a");
    }

    for (int j = 0; j < count; j++)
        free(matches[j]);
}
static void handle_tab(char *buf, int *pos, int last_was_tab) {
    buf[*pos] = '\0';

    if (!try_complete_builtin(buf, pos))
        try_complete_path(buf, pos, last_was_tab);
}

static void handle_backspace(char *buf, int *pos) {
    if (*pos > 0) {
        (*pos)--;
        printf("\b \b");
    }
}

static void handle_printable(char *buf, int *pos, char c) {
    buf[(*pos)++] = c;
    printf("%c", c);
}
void read_input(char *buf, int size) {
    enable_raw_mode();

    int i = 0;
    int last_was_tab = 0;
    char c;

    while (i < size - 1) {
        read(STDIN_FILENO, &c, 1);

        if (c == '\n') {
            break;
        }

        if (c == '\t') {
            handle_tab(buf, &i, last_was_tab);
            last_was_tab = 1;
            continue;
        }

        if (c == 127) {
            handle_backspace(buf, &i);
        } else {
            handle_printable(buf, &i, c);
        }

        last_was_tab = 0;
    }

    buf[i] = '\0';
    printf("\n");
    disable_raw_mode();
}