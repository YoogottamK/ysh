#include "shell.h"

#ifndef __PINFO_H
#define __PINFO_H

/*
 * Handles calls to pinfo command
 */
void pinfoHandler(Command c);

/*
 * implements the pinfo command
 */
void pinfo(char * pid);

#endif // __PINFO_H
