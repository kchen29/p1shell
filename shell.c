#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "shell.h"

/*
  print directory and then $
 */
void print_shell_prompt() {
  char buf[256];
  getcwd(buf, sizeof(buf));

  //if the current directory is the home directory, print tilde
  //otherwise, print the last directory of the current working dir
  char *dir;
  if (strcmp(buf, getenv("HOME")) == 0){
    dir = "~";
  }
  else{
    dir = 1 + strrchr(buf, '/');
  }
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
  if (!fgets(s, size, stdin)){
    exit(0);
  }
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
    //get rid of extra spaces
    if (strcmp(arg, "") == 0){
      continue;
    }
    args[i] = arg;
    i++;
  }
  args[i] = NULL;
  
  return args;
}

// takes ["ls", "-l", ";", "echo", "hello", NULL]
// returns [["ls", "-l", NULL], ["echo", "hello", NULL], NULL]
char ***sep_args(char **args, char *delim){
  char ***arrr = malloc(20 * sizeof(char **));

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
  forks and execs
 */
void do_command(char **args) {
  if (fork() == 0) {
    execvp(args[0], args);
    //can continue if command doesn't exist
    printf("%s: command not found\n", args[0]);
    exit(1);
  } else {
    int status;
    wait(&status);
    //do something here?
  }
}

//opens a pipe between the two arguments
//path acts as a temporary buffer, taking the output of fir using fgets, and sending it to sec using fprintf
void piper(char *fir, char *sec){
  FILE * fp1 = popen(fir, "r");
  FILE * fp2 = popen(sec, "w");
  char path[1024];
  while(fgets(path, sizeof(path), fp1)){
    fprintf(fp2, "%s", path);
  }
  pclose(fp1);
  pclose(fp2);
  return;
}


/*
  runs one command
  accounts for redirection

  ex: takes ["ls", "-a", "-l", NULL]
  and uses do_command
 */
void run_command(char **args) {
  //special case exit and cd
  if (strcmp(args[0], "exit") == 0) {
    exit(0);
  }
  else if (strcmp(args[0], "cd") == 0) {
    //if no other arguments, go to home directory
    if (!args[1]){
      chdir(getenv("HOME"));
    }
    else{
      chdir(args[1]);
    }
    return;
  }
  
  //for other commands, fork and exec; check for redirection
  
  //look at the second to last arg for "<" or ">"
  //(then the last arg will be a file)
  int len = 0;
  
  char *** temp;
  int t = 0;
  
  while (args[len]) {
    //check if there is a pipe, and split the commands if it exits
    if (strcmp(args[len], "|") == 0){
      temp = sep_args(args, "|");
      t = 1;
    }
    
    len++;
  }
  //combine the two 2d arrays into 2 1d arrays to be used with popen
   if (t){
    t = 0;
    char fir[1024];
    memset(fir, 0, 1024);
    char sec[1024];
    memset(sec, 0, 1024);
    while (temp[0][t]) {
      strcat(fir, temp[0][t]);
      if (temp[0][t + 1]){
	strcat(fir, " ");
      }
      t++;
    }
    t = 0;
    while (temp[1][t]) {
      strcat(sec, temp[1][t]);
      if (temp[1][t + 1]){
	strcat(sec, " ");
      }
      t++;
    }
    piper(fir, sec);
    return;
    }
  
  //check for redirection
  //if length is less than 3 -> no redirection
  if (len < 3) {
    do_command(args);
    return;
  }

  //redirection character should be the second to last
  char *redir = args[len - 2];
  //check if it is a redirection character
  if (strcmp(redir, ">") == 0) {
    //set the redirection to be null
    args[len - 2] = NULL;
    
    //open file
    char *file = args[len - 1];
    int fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    //dup
    int newfd = dup(1);
    dup2(fd, 1);
    
    do_command(args);
        
    //undo dup
    dup2(newfd, 1);
    close(newfd);
  } else if (strcmp(redir, "<") == 0) {
    args[len - 2] = NULL;

    char *file = args[len - 1];
    int fd = open(file, O_RDONLY, 0644);

    if (fd == -1) {
      printf("%s: No such file or directory\n", file);
      return;
    }
    
    int newfd = dup(0);
    dup2(fd, 0);

    do_command(args);

    dup2(newfd, 0);
    close(newfd); 
  } else {
    //if it isn't, then simply do it
    do_command(args);
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
    char ***arrr = sep_args(args, ";");
    
    run_commands(arrr);
    free(arrr);
    free(args);
  }
  return 0;
}
