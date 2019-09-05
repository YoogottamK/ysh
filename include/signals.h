#include "shell.h"

/*
 * signal handler for when background process quits
 */
void bgProcessExit(int sig);

void stopBgProcess(int sig);
