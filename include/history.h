#include "shell.h"
#include "utils.h"

#ifndef __HISTORY_H
#define __HISTORY_H

/*
 * Manages updating history
 */
void updateHistory(Piped * piped);

/*
 * Handles the history command
 */
void historyHandler(Command c);

/*
 * Counts number of up arrows in the string
 */
int getUpArrowCount(char * str);

#endif // __HISTORY_H
