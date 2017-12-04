void print_shell_prompt();
void get_input(char *s, int size);
char **parse_args(char *line);
char ***sep_args(char **args, char *delim);
void do_command(char **args);
void run_command(char **args);
void run_commands(char ***arrr);
