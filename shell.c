#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  wrapper for fgets

  gets size characters from stdin
  exit if eof character is the only character inputted
  remove newline from fgets
 */
void get_input(char *s, int size) {
  //fgets returns null when end of file character occurs
  //while no characters have been read
  if (!fgets(s, size, stdin))
    exit(0);
  //remove newline
  *(strchr(s, '\n')) = 0;
}

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

/*
  runs one command
  
  ex: takes ["ls", "-a", "-l", NULL]
  and forks and execs it
 */
void run_command(char **args) {
  //special case exit and cd
  if (strcmp(args[0], "exit") == 0) {
    exit(0);
  } else if (strcmp(args[0], "cd") == 0) {
    //if no other arguments, go to home directory
    //may not work on platforms other than linux (have not tested)
    if (!args[1])
      chdir(getenv("HOME"));
    else
      chdir(args[1]);
    return;
  }

  //for other commands, fork and exec
  if (fork() == 0) {
    execvp(args[0], args);
  } else {
    int status;
    wait(&status);
    //do something here?
  }
}

int main() {
  char s[256];
  while(1){
    get_input(s, sizeof(s));

    char **args = parse_args(s);

    run_command(args);
    free(args);
  }
  return 0;
}
