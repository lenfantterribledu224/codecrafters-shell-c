#include <string.h>
#include "matches.h"
#include <stdio.h>
#include <unistd.h>
#include "completion.h"
#include "handle.h"
#include "terminal.h"

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