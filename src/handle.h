#ifndef HANDLER_H
#define HANDLER_H

void handle_tab(char *buf, int *pos, int last_was_tab);
void handle_backspace(char *buf, int *pos);
void handle_printable(char *buf, int *pos, char c);

#endif