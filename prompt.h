#include "shell.h"

#ifndef __PROMPT_H
#define __PROMPT_H

/*
 * This replaces "HOME" with ~
 */
char * replaceWithTilda(char * cwd);

/*
 * This function prints the prompt
 */
void makePrompt();

#endif // __PROMPT_H
