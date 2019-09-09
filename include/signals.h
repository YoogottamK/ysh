#include "shell.h"

#ifndef __SIGNALS_H
#define __SIGNALS_H

/*
 * signal handler for when background process quits
 */
void bgProcessExit(int sig);

/*
 * signal handler for when background process quits
 */
void stopBgProcess(int sig);

#endif // __SIGNALS_H
