#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "utils.h"

int main(int argc, char *argv[]) {
setbuf(stdout, NULL);
char command[1024];

while(1){

  printf("$ ");
  fgets(command, sizeof(command), stdin);
  command[strcspn(command, "\n")] = '\0';

  char *args[1024];
  int nargs = 0;
  parse_args(command, args, &nargs);


  if (strcmp(args[0], "exit") == 0)  {
    break;
  } else if (strcmp(args[0], "echo") == 0) {
    do_echo(args, nargs);
  } else if (strcmp(args[0], "type") == 0) {
    do_type(args, nargs);
  } else if (strcmp(args[0], "pwd") == 0) {
    do_pwd(args, nargs);
  } else if (strcmp(args[0], "cd") == 0) {
    do_cd(args, nargs);
  } else {
    do_execute(args, nargs);
  }
}
  return 0;
}