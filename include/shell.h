#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "list.h"

#ifndef __SHELL_H
#define __SHELL_H

// colors
#define COL_FG_BLK "\033[30;1m"
#define COL_FG_RED "\033[31;1m"
#define COL_FG_GRN "\033[32;1m"
#define COL_FG_YLW "\033[33;1m"
#define COL_FG_BLU "\033[34;1m"
#define COL_FG_MGN "\033[35;1m"
#define COL_FG_CYN "\033[36;1m"
#define COL_FG_WHT "\033[37;1m"

#define COL_BG_BLK "\033[40;1m"
#define COL_BG_RED "\033[41;1m"
#define COL_BG_GRN "\033[42;1m"
#define COL_BG_YLW "\033[43;1m"
#define COL_BG_BLU "\033[44;1m"
#define COL_BG_MGN "\033[45;1m"
#define COL_BG_CYN "\033[46;1m"
#define COL_BG_WHT "\033[47;1m"

#define COL_RST "\033[0m"

#define MAX_LEN 1024

/*
 * Command: the most basic unit,
 *  cmd arg1 arg2 ... > outFile < inFile &
 */
typedef struct Command {
    char * command;
    int argc;
    char ** args;

    bool bg;

    char * inp;
    char * out;

    bool append;
} Command;

/*
 * Piped: contains various commands piped into one another
 *  cmd1 | cmd2 | cmd3
 */
typedef struct Piped {
    int n;
    Command * commands;
} Piped;

/*
 * History: for storing history
 */
typedef struct History {
    char history[20][MAX_LEN];
    int index;
} History;

char HOME[MAX_LEN];

// history object
History h;

// root pointer to the linked list of open bg processes
Node * procList;

// stores the number of times the up key was pressed
int upCount;

// stores the foreground process's pid
pid_t fgPid;

// stores the whole foreground command
Command fgCommand;

#endif // __SHELL_H
