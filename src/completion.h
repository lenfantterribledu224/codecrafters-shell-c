#ifndef COMPLETION_H
#define COMPLETION_H
typedef struct {
    char *buf;
    int *pos;
} EditState;
int try_complete_builtin(EditState *state);
void try_complete_path(EditState *state, int last_was_tab);
void complete_partial(EditState *state, const char *ref, int prefix_len);
#endif // COMPLETION_H