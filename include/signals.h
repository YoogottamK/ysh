#include "shell.h"

#ifndef __SIGNALS_H
#define __SIGNALS_H

/*
 * signal handler for when background process quits
 */
void bgProcessExit(int sig);

/*
 * Handler for SIGINT, when Ctrl+C is pressed
 */
void ctrlcHandler(int sig);

/*
 * Handler for SIGTSTP, when Ctrl+Z is pressed
 */
void ctrlzHandler(int sig);

#endif // __SIGNALS_H
