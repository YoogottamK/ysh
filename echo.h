#include "shell.h"

#ifndef __ECHO_H
#define __ECHO_H

/*
 * Handles calls to echo
 */
void echoHandler(Command c);

/*
 * the echo function
 */
void echo(char ** args, int n);

#endif // __ECHO_H
