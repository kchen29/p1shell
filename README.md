# p1shell

Features:
- [x] Read a line at a time, parse the line to separate the command from its arguments. It should then fork and exec the command. The parent process should wait until the exec'd program exits and then it should read the next command. Assumes no more than 19 arguments

- [x] Read and separate multiple commands on one line with `;`. That is, the line `ls -l ; echo hello` should first run the command `ls -l` and then `echo hello`.
- [x] Implement simple redirection using `>` (redirecting stdout) and `<` (redirecting stdin). Assume redirection is of the form `<command> <redirection character> <file>`. Only 1 redirection per command (separated by `";"`).
- [x] Implement simple pipes `|`. Pipe is of the form `<command> | <command>`. Only 1 pipe per command.

Bugs:
* `cd` doesn't work with ~ for the home directory (no tilde expansion)

Fixed bugs:
* `cd` with no other args doesn't change to the home directory
* seg fault when only EOF character is entered as input




Function Headers:
* void print_shell_prompt();
   * if the current directory is the home directory, print tilde
   * otherwise, print the last directory of the current working dir
  
* void get_input(char *s, int size); 
   * Input: char array to store users input in and size of that array
   * wrapper for fgets
   * gets size characters from stdin
   * exit if nothing is inputted
   * remove newline from fgets

* char **parse_args(char *line);
   * input: user's input as a char array to be parsed
   * output: user's input as an array of char arrays by each argument
   * assume 1 space between each argument
   * assume no more than 19 args
   * assume correctly formatted
   * ex: takes "ls -a -l"
   * returns array ["ls", "-a", "-l", NULL]
  
  
* char ***sep_args(char **args);
   * input: user's input as an array of char arrays by each argument
   * output: array of arrays of char arrays, split based each individual command by semicolons
   * takes ["ls", "-l", ";", "echo", "hello", NULL]
   * returns [["ls", "-l", NULL], ["echo", "hello", NULL], NULL]
   
   
* void do_command(char **args);
   * forks off a child process and makes it do the exec'ing
   
   
* void run_command(char **args);
   * input: the command to run, in an array of strings separated by spaces
   * runs a single command
   * accounts for cd, exit, <, and >
   
   
* void run_commands(char ***arrr);
   * input: the array of arrays of strings based on number of commands, and number of spaces in each
   * wrapper for run_command

* void piper(char *fir, char *sec);
   * input: two commands to be piped together
   * the output of fir is used as the input of sec
   * opens a pipe between the two arguments
   * "path" acts as a temporary buffer, taking the output of fir using fgets, and sending it to sec using fprintf

* main()
   * Runs an infinite while loop that takes user's input and runs each line they type
