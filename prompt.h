#include "shell.h"

#ifndef __PROMPT_H
#define __PROMPT_H

/*
 * This gets the current working dir, replacing HOME with ~
 */
char * getDir();

/*
 * This function prints the prompt
 */
void makePrompt();

#endif // __PROMPT_H
