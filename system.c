#include <sys/wait.h>

#include "system.h"

void systemCommand(Command c) {
    char ** args = (char**) malloc((c.argc + 2) * sizeof(char*));
    args[0] = c.command;

    for(int i = 0; i < c.argc; i++)
        args[i+1] = c.args[i];

    args[c.argc + 1] = 0;

    pid_t pidChild = fork();

    if(pidChild < 0) {
        printf("Error in fork\n");

        free(args);
        return;
    }

    if(pidChild == 0) {
        if(execvp(c.command, args) < 0)
            perror(c.command);

        exit(0);
    } else {
        wait(0);
    }

    free(args);
}
