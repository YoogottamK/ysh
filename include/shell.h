#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

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

typedef struct Command {
    int argc;
    char * command;
    char ** args;
    bool bg;
} Command;

typedef struct Parsed {
    int n;
    Command * commands;
} Parsed;

typedef struct History {
    char history[20][MAX_LEN];
    int index;
} History;

char HOME[MAX_LEN];

History h;
Node * procList;

#endif // __SHELL_H
