// src/main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils.h"
#include <unistd.h>
#include "input.h"
#include "pipeline.h"
#include "navigation.h"

int main(int argc, char *argv[]) {
setbuf(stdout, NULL);
char command[1024];
char *history[1024];
int history_count = 0;
int append_start = 0;
while(1){
  printf("$ ");
  read_input(command, sizeof(command),history, history_count);


// inside the loop, right after read_input:
 history[history_count++] = strdup(command);

  char *args[1024];
  int nargs = 0;
  parse_args(command, args, &nargs);
  // check for pipe first
  int has_pipe = 0;
  for (int i = 0; i < nargs; i++) {
      if (strcmp(args[i], "|") == 0) {
          has_pipe = 1;
          break;
      }
  }

  if (has_pipe) {
      // handle pipeline, then skip everything below
      do_pipeline(args, nargs);
      free_args(args, nargs);
      continue;

  }
  int redirected_fd = -1;  // default to stdout
  int saved_fd = handle_redirection(args, &nargs, &redirected_fd);


  if (strcmp(args[0], "exit") == 0)  {
    break;
  } else if (strcmp(args[0], "history") == 0) {
      if (nargs > 2 && strcmp(args[1], "-w") == 0) {
        history_write_file(args[2], history, history_count);
    } else if (nargs > 2 && strcmp(args[1], "-r") == 0) {
        history_read_file(args[2], history, &history_count, &append_start);
    } else if (nargs > 2 && strcmp(args[1], "-a") == 0) {
        history_append_file(args[2], history, history_count, &append_start);
    } else {
      int limit = (nargs > 1) ? atoi(args[1]) : 0;
      do_history(history, history_count, limit);
    }
  }
  else if (is_builtin(args[0])) {
    run_builtin(args, nargs);
  }  else {
    do_execute(args, nargs);
  }
  if (saved_fd != -1) {
    dup2(saved_fd, redirected_fd);
    close(saved_fd);
}
  free_args(args, nargs);
}

  return 0;
}