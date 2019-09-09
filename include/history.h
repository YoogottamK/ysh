#include "shell.h"
#include "utils.h"

#ifndef __HISTORY_H
#define __HISTORY_H

/*
 * manages updating history
 */
void updateHistory(Command c);

/*
 * handles the history command
 */
void historyHandler(Command c);

#endif // __HISTORY_H
