#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
setbuf(stdout, NULL);
char command[1024];
while(1){
  printf("$ ");
  fgets(command, sizeof(command), stdin);
  command[strcspn(command, "\n")] = '\0';
  char *cmd = strncmp(command, "echo ", 5) == 0 ? "echo" : command;
  char *args = cmd + 5; // Move past "echo "
  if (strcmp(cmd, "exit") == 0) {
    break;
} else if (strcmp(cmd, "echo") == 0) {
    printf("%s\n", args);
} else {
  printf("%s: command not found\n", cmd);
}
}
  return 0;
}
