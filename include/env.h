#include "shell.h"

#ifndef __ENV_H
#define __ENV_H

/*
 * handles calls to the setenv command
 */
void setEnvHandler(Command c);

/*
 * handles calls to the unsetenv command
 */
void unsetEnvHandler(Command c);

#endif // __ENV_H
