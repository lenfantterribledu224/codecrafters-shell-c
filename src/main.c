#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils.h"
#include <unistd.h>
#include "input.h"
#include "pipeline.h"

int main(int argc, char *argv[]) {
setbuf(stdout, NULL);
char command[1024];

while(1){
  printf("$ ");
  read_input(command, sizeof(command));

  char *args[1024];
  int nargs = 0;
  parse_args(command, args, &nargs);
  // check for pipe first
  int pipe_pos = -1;
  for (int i = 0; i < nargs; i++) {
      if (strcmp(args[i], "|") == 0) {
          pipe_pos = i;
          break;
      }
  }

  if (pipe_pos != -1) {
      // handle pipeline, then skip everything below
      do_pipeline(args, pipe_pos);
      free_args(args, nargs);
      continue;

  }
  int redirected_fd = -1;  // default to stdout
  int saved_fd = handle_redirection(args, &nargs, &redirected_fd);


  if (strcmp(args[0], "exit") == 0)  {
    break;
  } else if (is_builtin(args[0])) {
    run_builtin(args, nargs);
  } else {
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