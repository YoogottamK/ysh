#include "shell.h"

#ifndef __LS_H
#define __LS_H

typedef struct {
    bool a;
    bool l;
} lsOpts;

/*
 * Handles calls to ls
 */
void lsHandler(Command c);

/*
 * The ls function
 */
void ls(char * path, lsOpts opts);

#endif // __LS_H
