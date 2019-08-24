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
 - `cd.c`: Contains code t=for the `cd` builtin
 - `echo.c`: Contains code for the `echo` builtin
 - `external.c`: Contains code that executes system commands [fg/bg]
 - `history.c`: Contains code for the `history` builtin
 - `ls.c`: Contains code for the `ls` builtin
 - `nightswatch.c`: Contains code for the `nightswatch` builting
 - `parse.c`: Contains code which parses the input given to the shell, breaks it down into commands and it's arguments
 - `pcwd.c`: Contains code for the `pwd` builtin
 - `pinfo.c`: Contains code for the `pinfo` builtin
 - `prompt.c`: Contains code which displays the shell prompt
 - `README.md`: You are reading this
 - `shell.c`: Contains the main function, probably the smallest file here
 - `signals.c`: Contains signal handlers which were used
 - `utils.c`: Contains some functions which were used across all the files
