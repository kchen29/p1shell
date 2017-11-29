# p1shell

Features:
- [x] Read a line at a time, parse the line to separate the command from its arguments. It should then fork and exec the command. The parent process should wait until the exec'd program exits and then it should read the next command.

- [ ] Read and separate multiple commands on one line with `;`. That is, the line `ls -l ; echo hello` should first run the command `ls -l` and then `echo hello`.
- [ ] Implement simple redirection using `>` (redirecting stdout) and `<` (redirecting stdin).
- [ ] Implement simple pipes, that is: `ls | wc` would run `ls` and use the output from `ls` as the input for `wc`.

Bugs:
* `cd` doesn't work with ~ for the home directory

Fixed bugs:
* `cd` with no other args doesn't change to the home directory
* seg fault when only EOF character is entered as input