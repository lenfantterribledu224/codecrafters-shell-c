// matches.h
#ifndef MATCHES_H
#define MATCHES_H

typedef struct {
    char *items[1024];
    int count;
} MatchList;


void show_matches(MatchList *matches, const char *buf);
void find_file_matches(const char *dir, const char *prefix, MatchList *matches);
int compute_lcp(MatchList *matches);

#endif // MATCHES_H