#include "shell.h"

typedef struct {
    bool a;
    bool l;
} lsOpts;

/*
 * Handles calls to ls
 */
void lsHandler(Command c);

/*
 * The ls function
 */
void ls(char * path, lsOpts opts);
