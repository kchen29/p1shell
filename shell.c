#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  ex: takes "ls -a -l"
  returns array ["ls", "-a", "-l", NULL]

  assume 1 space between each argument
  assume no more than 19 args
  assume correctly formatted
*/

char **parse_args(char *line) {
  //pointer to "array"; free this later
  char **args = malloc(20 * sizeof(char *));

  int i = 0;
  while (line) {
    char *arg = strsep(&line, " ");
    args[i] = arg;
    i++;
  }
  args[i] = NULL;
  
  return args;
}

int main() {
  char s[256];
  while(1){
    //get data
    fgets(s, sizeof(s), stdin);
    //remove newline from fgets
    *(strchr(s, '\n')) = 0;
    
    //parse
    char **args = parse_args(s);
    
    if (strcmp(args[0], "exit") == 0) {
      return 0;
    } else if (strcmp(args[0], "cd") == 0) {
      chdir(args[1]);
    } else {
      if (fork() == 0) {
	execvp(args[0], args);
      } else {
	int status;
	wait(&status);
	free(args);
      }
    }
  }
  return 0;
}
