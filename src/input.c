#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "shell.h"
#include "input.h"

void read_input(char *buf, int size) {
    struct termios orig, raw;
    tcgetattr(STDIN_FILENO, &orig);
    raw = orig;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    int i = 0;
    char c;

    while (i < size - 1) {
        read(STDIN_FILENO, &c, 1);

        if (c == '\n') {
            break;
        } else if (c == '\t') {
            buf[i] = '\0';
            int matched = 0;
            for (int j = 0; j < num_builtins; j++) {
                 if (strncmp(buf, builtins[j].name, i) == 0) {
                 printf("%s ", builtins[j].name + i);
                 strcpy(buf, builtins[j].name);
                 strcat(buf, " ");
                 i = strlen(buf);
                 matched = 1;
                 break;
        }
    }
    if (!matched) printf("\a");
  }
         else if (c == 127) {  // backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            buf[i++] = c;
            printf("%c", c);
        }
    }

    buf[i] = '\0';
    printf("\n");
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}