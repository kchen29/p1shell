# p1shell

Features:
- [x] Read a line at a time, parse the line to separate the command from its arguments. It should then fork and exec the command. The parent process should wait until the exec'd program exits and then it should read the next command.

- [x] Read and separate multiple commands on one line with `;`. That is, the line `ls -l ; echo hello` should first run the command `ls -l` and then `echo hello`.
- [x] Implement simple redirection using `>` (redirecting stdout) and `<` (redirecting stdin).


Attempted:
* Pipes did not work

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
   
void run_command(char **args);
void run_commands(char ***arrr);
