#include <unistd.h>
#include "completion.h"
#include "matches.h"
#include <stdio.h>
#include "handle.h"
#include <string.h>

 void handle_printable(char *buf, int *pos, char c) {
    buf[(*pos)++] = c;
    printf("%c", c);
}

static void handle_backspace(char *buf, int *pos) {
    if (*pos > 0) {
        (*pos)--;
        printf("\b \b");
    }
}
 void handle_tab(char *buf, int *pos, int last_was_tab) {
    buf[*pos] = '\0';

    char *last_space = strrchr(buf, ' ');
    if (last_space != NULL) {
        char *arg = last_space + 1;
        char dir[1024] = ".";
        char *prefix = arg;
        char *last_slash = strrchr(arg, '/');

        if (last_slash != NULL) {
            int dir_len = last_slash - arg + 1;
            strncpy(dir, arg, dir_len);
            dir[dir_len] = '\0';
            prefix = last_slash + 1;
        }

        MatchList matches = { .count = 0 };
        find_file_matches(dir, prefix, &matches);
        int prefix_len = strlen(prefix);

        if (matches.count == 1) {
        printf("%s ", matches.items[0] + prefix_len);
        fflush(stdout);
        char *write_point = (last_slash != NULL) ? last_slash + 1 : arg;
        strcpy(write_point, matches.items[0]);
        strcat(buf, " ");
        *pos = strlen(buf);
    } else if (matches.count > 1) {
        int lcp = compute_lcp(&matches);
        if (lcp > prefix_len) {
            for (int k = prefix_len; k < lcp; k++)
                printf("%c", matches.items[0][k]);
            fflush(stdout);
            char *write_point = (last_slash != NULL) ? last_slash + 1 : arg;
            strncpy(write_point, matches.items[0], lcp);
            write_point[lcp] = '\0';
            *pos = strlen(buf);
        } else if (last_was_tab) {
            show_matches(&matches, buf);
        } else {
            write(STDOUT_FILENO, "\x07", 1);
        }
    }

    } else {
        EditState state = { buf, pos };
        if (!try_complete_builtin(&state))
            try_complete_path(&state, last_was_tab);
    }
}

