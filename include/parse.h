#include "shell.h"

#ifndef __PARSE_H
#define __PARSE_H

void dump(Parsed p);

/*
 * Trims leading and trailing spaces by advancing the pointer
 *  and introducing '\0' in the end
 */
char * stripSpaces(char * str);

/*
 * Counts the number of substrings given in the string if split by delim
 */
int count(char * str, char delim);

/*
 * The high level function that
 *  - breaks down a string into ';' sep commands, then into individual parameters
 *  - replaces ~ with home
 */
Parsed parse(char * str);

#endif // __PARSE_H
