#include "shell.h"

#ifndef __PARSE_H
#define __PARSE_H

/*
 * frees memory once command execution is over
 */
void dump(Parsed p);

/*
 * The high level function that
 *  - breaks down a string into ';' sep commands, then into individual parameters
 *  - replaces ~ with home
 */
Parsed parse(char * str);

#endif // __PARSE_H
