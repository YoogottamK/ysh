#include <sys/wait.h>

#include "external.h"
#include "signals.h"

void systemCommand(Command c) {
    char ** args = (char**) malloc((c.argc + 2) * sizeof(char*));
    int status;
    args[0] = c.command;

    for(int i = 0; i < c.argc; i++)
        args[i+1] = c.args[i];

    args[c.argc + 1] = 0;

    pid_t pidChild = fork();

    if(pidChild < 0) {
        fprintf(stderr, "Error in fork\n");

        free(args);
        return;
    }

    if(pidChild == 0) {
        // child process


        if(c.bg) {
            setpgid(0, 0);
            signal(SIGTTIN, stopBgProcess);
        }

        if(execvp(c.command, args) < 0) {
            fprintf(stderr, "Error: command '%s' not found\n", c.command);
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        if(c.bg) {
            signal(SIGCHLD, bgProcessExit);

            int index = 0;

            for(; index < PROC_LIST; index++)
                if(p[index].pid == -1)
                    break;

            // could find an empty spot
            if(index != PROC_LIST) {
                p[index].procName[0] = 0;
                strcpy(p[index].procName, c.command);
                p[index].pid = pidChild;
            }
        }
        else
            waitpid(-1, &status, 0);
    }

    free(args);
}
