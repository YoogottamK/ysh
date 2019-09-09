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

#endif // __LS_H
