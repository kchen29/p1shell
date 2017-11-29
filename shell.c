#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
  print directory and then $
 */
void print_shell_prompt() {
  char buf[256];
  getcwd(buf, sizeof(buf));

  //if the current directory is the home directory, print tilde
  //otherwise, print the last directory of the current working dir
  char *dir;
  if (strcmp(buf, getenv("HOME")) == 0)
    dir = "~";
  else
    dir = 1 + strrchr(buf, '/');
  
  printf("%s$ ", dir);
}

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

// takes ["ls", "-l", ";", "echo", "hello", NULL]
// returns [["ls", "-l", NULL], ["echo", "hello", NULL], NULL]
char ***sep_args(char **args){
  char ***arrr = malloc(20 * sizeof(char **));

  char *delim = ";";
  int i;
  int j = 1;
  arrr[0] = &args[0];
  for (i = 0; args[i]; i++){
    if (strcmp(args[i], delim) == 0){
      args[i] = NULL;
      arrr[j] = &args[i + 1];
      j++;
    }
  }
  arrr[j] = NULL;
  return arrr;
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

/*
  wrapper for run_command
  takes [["ls", "-l", NULL], ["echo", "hello", NULL], NULL]
  and does run_command for each command
 */
void run_commands(char ***arrr) {
  int i;
  for (i = 0; arrr[i]; i++) {
    run_command(arrr[i]);
  }
}

int main() {
  char s[256];
  while(1){
    print_shell_prompt();
    
    get_input(s, sizeof(s));

    char **args = parse_args(s);
    char ***arrr = sep_args(args);
    
    run_commands(arrr);
    free(arrr);
    free(args);
  }
  return 0;
}
