#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "shell.h"
#include "completion.h"
#include "utils.h"


 static void  complete_word(EditState *state, const char *full) {
    int cur = *state->pos;
    printf("%s ", full + cur);
    strcpy(state->buf, full);
    strcat(state->buf, " ");
    *state->pos = strlen(state->buf);
}

int try_complete_builtin(EditState *state) {
    for (int j = 0; j < num_builtins; j++) {
        if (strncmp(state->buf, builtins[j].name, *state->pos) == 0) {
            complete_word(state, builtins[j].name);
            return 1;
        }
    }
    return 0;
}

 static void complete_partial(EditState *state, const char *ref, int prefix_len) {
    for (int k = *state->pos; k < prefix_len; k++)
        printf("%c", ref[k]);
    fflush(stdout);

    strncpy(state->buf, ref, prefix_len);
    state->buf[prefix_len] = '\0';
    *state->pos = prefix_len;
}

 void try_complete_path(EditState *state, int last_was_tab) {
    char *matches[1024];
    int count = 0;

    find_in_path_prefix(state->buf, matches, &count);

   if (count == 1) {
    complete_word(state, matches[0]);
}  else if (count > 1) {
    int prefix_len = compute_lcp(matches, count);
    if (prefix_len > *state->pos) {          // caller decides whether to call
        complete_partial(state, matches[0], prefix_len);  // function just does the work
    } else if (last_was_tab) {
        show_matches(matches, count, state->buf);
    } else {
        write(STDOUT_FILENO, "\x07", 1);
    }
} else {
    write(STDOUT_FILENO, "\x07", 1);
}

for (int j = 0; j < count; j++)
    free(matches[j]);
}


