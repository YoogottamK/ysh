#include "shell.h"

#ifndef __UTILS_H
#define __UTILS_H

/*
 * This replaces "HOME" with ~
 */
char * replaceWithTilda(char * cwd);

/*
 * gets the nth text segment from a string seperated by spaces
 */
char * getArg(char * str, int n);

/*
 * opens file with name 'file' inside dir 'dir' and returns the
 *  fd for it
 */
int openFile(char * dir, char * file);

#endif // __UTILS_H
