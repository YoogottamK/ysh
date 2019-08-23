#include "shell.h"

#ifndef __CD_H
#define __CD_H

/*
 * Handles the cd command and it's arguments
 */
void cdHandler(Command c);

/*
 * Changes the directory to path
 *  if path is not given, it goes to home directory
 */
void cd(const char * path);

#endif // __CD_H
