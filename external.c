#include <sys/wait.h>
#include <stdlib.h>

#include "external.h"
#include "list.h"
#include "utils.h"

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
        setpgid(0, 0);

        if(execvp(c.command, args) < 0) {
            fprintf(stderr, "Error: command '%s' not found\n", c.command);
            exit(EXIT_FAILURE);
        }

        // if exec reaches this point, it means execvp failed
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        if(c.bg) {
            Process p;
            p.pid = pidChild;
            p.name = (char *) malloc(strlen(c.command) + 1);
            strcpy(p.name, getFullCommand(c));

            procList = insert(procList, p);
        } else {
            fgPid = pidChild;
            fgCommand = c;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(0, fgPid);

            waitpid(fgPid, &status, WUNTRACED);

            tcsetpgrp(0, getpgid(0));

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if(WIFSTOPPED(status)) {
                printf("\nSuspended\t%s [%d]\n", getFullCommand(fgCommand), fgPid);

                Process p;
                p.pid = fgPid;
                p.name = (char *) malloc(strlen(fgCommand.command) + 1);
                strcpy(p.name, getFullCommand(fgCommand));

                procList = insert(procList, p);

                fgPid = -1;
            }

            fgPid = -1;
        }
    }

    free(args);
}
