#ifndef NAVIGATION_H
#define NAVIGATION_H
int read_arrow_key();
int navigate_history(int current_index, int direction, int history_count);
void replace_line(char *buf, int *pos, const char *new_text);
void handle_arrow(char *buf, int *pos, char **history, int history_count, int *history_index);
#endif // NAVIGATION_H