// src/navigation.c
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "navigation.h"
int read_arrow_key() {
    char seq[2];
    read(STDIN_FILENO, &seq[0], 1);
    read(STDIN_FILENO, &seq[1], 1);
    if (seq[0] == '[' && seq[1] == 'A') return 1;
    if (seq[0] == '[' && seq[1] == 'B') return -1;
    return 0;
}

int navigate_history(int current_index, int direction, int history_count) {
    int new_index = current_index - direction;
    if (new_index < 0) return 0;
    if (new_index > history_count) return history_count;
    return new_index;
}

void replace_line(char *buf, int *pos, const char *new_text) {
    // Move cursor back to start of input
    for (int i = 0; i < *pos; i++)
        write(STDOUT_FILENO, "\b", 1);
    // Clear the line
    for (int i = 0; i < *pos; i++)
        write(STDOUT_FILENO, " ", 1);
    // Move cursor back again
    for (int i = 0; i < *pos; i++)
        write(STDOUT_FILENO, "\b", 1);
    // Write new text
    int len = strlen(new_text);
    write(STDOUT_FILENO, new_text, len);
    // Update buffer
    strcpy(buf, new_text);
    *pos = len;
}

void handle_arrow(char *buf, int *pos, char **history, int history_count, int *history_index) {
    int arrow = read_arrow_key();
    if (arrow != 0) {
        *history_index = navigate_history(*history_index, arrow, history_count);
        const char *text = (*history_index < history_count) ? history[*history_index] : "";
        replace_line(buf, pos, text);
    }
}
void history_read_file(const char *path, char **history, int *history_count) {
    FILE *f = fopen(path, "r");
    if (!f) return;

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            history[(*history_count)++] = strdup(line);
        }
    }
    fclose(f);
}

void history_write_file(const char *path, char **history, int history_count) {
    FILE *f = fopen(path, "w");
    if (!f) return;

    for (int i = 0; i < history_count; i++) {
        fprintf(f, "%s\n", history[i]);
    }
    fclose(f);
}

void history_append_file(const char *path, char **history, int history_count) {
    FILE *f = fopen(path, "a");
    if (!f) return;

    for (int i = 0; i < history_count; i++) {
        fprintf(f, "%s\n", history[i]);
    }
    fclose(f);
}