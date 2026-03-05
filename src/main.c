#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(int argc, char *argv[]) {
setbuf(stdout, NULL);
char command[1024];
while(1){
  printf("$ ");
  fgets(command, sizeof(command), stdin);
  command[strcspn(command, "\n")] = '\0';
  if (strcmp(command, "exit") == 0) {
    break;
  } else if (strncmp(command, "echo ", 5) == 0) {
    do_echo(command + 5);
  } else if (strncmp(command, "type ", 5) == 0) {
    do_type(command + 5);
  } else {
    printf("%s: command not found\n", command);
  }
}
  return 0;
}