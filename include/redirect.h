#include "shell.h"

#ifndef __REDIRECT_H
#define __REDIRECT_H

int stdoutSave, // fd for restoring stdout after redir is over
    stdinSave,  // fd for restoring stdin after redir is over
    inFd,       // new fd for input file
    outFd;      // new fd for output file

/*
 * Sets up the new input/output file for commands
 */
void redirectBegin(Command c);

/*
 * Restores stdin, stdout after command is done
 */
void redirectRestore();

#endif // __REDIRECT_H
