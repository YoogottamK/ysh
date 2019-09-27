# ysh
It's a shell

## Introduction
This is a very basic implementation of shell in C.

## Functionality
### Part1
 - [x] Support for semicolon seperated commands, not affected by whitespace
 - [x] Shell builtins: 
    - [x] `cd`
    - [x] `pwd`
    - [x] `echo`
    - [x] `exit`
    - [x] `history`
    - [x] `pinfo`
    - [x] `nightswatch`
    - [x] `ls -[al] [dirs..]`
 - [x] Execution of system commands with or without arguments, in background or foreground
 - [x] Notifying when and how the background process terminated
 - [x] *some other stuff else I didn't get the time to write because of the deadline*
### Part2
 - [x] Input and output redirection, including appending to a file
 - [x] Piping of commands
 - [x] Some more shell builtins:
    - [x] `setenv`
    - [x] `unsetenv`
    - [x] `jobs`
    - [x] `kjob`
    - [x] `fg`
    - [x] `bg`
    - [x] `overkill`
    - [x] `quit`
    - [x] `cronjob`
 - [x] Handling `SIGINT` [C-c] and `SIGTSTP` [C-z] signals
 - [x] Command recall using up arrow key

## Build instructions
Simply run
```
$ make
$ ./ysh
```
You'll see a lot of files. To remove them, you can run
```
$ make clean
```

## File structure
 - `bg.c`: Contains code for the `bg` builtin
 - `cd.c`: Contains code for the `cd` builtin
 - `cronjob.c`: Contains code for the `cronjob` builtin
 - `echo.c`: Contains code for the `echo` builtin
 - `env.c`: Contains code for the `setenv`, `unsetenv` builtins
 - `external.c`: Contains code that executes system commands [fg/bg]
 - `fg.c`: Contains code for the `fg` builtin
 - `history.c`: Contains code for the `history` builtin
 - `jobs.c`: Contains code for the `jobs` builtin
 - `kjob.c`: Contains code for the `kjob` builtin
 - `list.c`: Contains functions related to linked list
 - `ls.c`: Contains code for the `ls` builtin
 - `nightswatch.c`: Contains code for the `nightswatch` builtin
 - `overkill.c`: Contains code for the overkill builtin
 - `parse.c`: Contains code which parses the input given to the shell, breaks it down into commands and it's arguments
 - `pcwd.c`: Contains code for the `pwd` builtin
 - `pinfo.c`: Contains code for the `pinfo` builtin
 - `piping.c`: Contains code for implementing pipes
 - `prompt.c`: Contains code which displays the shell prompt
 - `README.md`: You are reading this
 - `redirect.c`: Contains code for implementing redirections
 - `shell.c`: Contains the main function, probably the smallest file here
 - `signals.c`: Contains signal handlers which were used
 - `utils.c`: Contains some functions which were used across all the files
