#include "matches.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>

 int compute_lcp(MatchList *matches) {
    int prefix_len = strlen(matches->items[0]);
    for (int j = 1; j < matches->count; j++) {
        int k = 0;
        while (k < prefix_len && matches->items[j][k] == matches->items[0][k])
            k++;
        prefix_len = k;
    }
    return prefix_len;
}

static void sort_matches(MatchList *matches) {
    for (int a = 0; a <matches->count - 1; a++)
        for (int b = a + 1; b < matches->count; b++)
            if (strcmp(matches->items[a], matches->items[b]) > 0) {
                char *tmp = matches->items[a];
                matches->items[a] = matches->items[b];
                matches->items[b] = tmp;
            }
}

 void find_file_matches(const char *dir, const char *prefix, MatchList *matches) {
    DIR *d = opendir(dir);
    if (d == NULL) return;

    int prefix_len = strlen(prefix);
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (strncmp(entry->d_name, prefix, prefix_len) == 0) {
            if (entry->d_type == DT_DIR) {
                char dir_name[1024];
                snprintf(dir_name, sizeof(dir_name), "%s/", entry->d_name);
                matches->items[matches->count++] = strdup(dir_name);
            } else {
                matches->items[matches->count++] = strdup(entry->d_name);
            }
}
    }
    closedir(d);
}

 void show_matches(MatchList *matches, const char *buf) {
    sort_matches(matches);
    printf("\n");
    for (int j = 0; j < matches->count; j++) {
        printf("%s", matches->items[j]);
        if (j < matches->count - 1)
            printf("  ");
    }
    printf("\n$ %s", buf);
}