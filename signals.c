#include <sys/wait.h>

#include "signals.h"
#include "prompt.h"
#include "utils.h"

void bgProcessExit(int sig) {
    int status;

    pid_t pid = waitpid(-1, &status, WNOHANG);

    if(pid > 0) {
        Process p = { .pid = pid, .name = 0 },
                proc;

        proc = find(procList, p);

        char * procName = proc.pid < 0 ? "Process" : getArg(proc.name, 1);

        if(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
            fprintf(stderr, COL_FG_YLW "\r%s with pid %d exited normally\n" COL_RST, procName, pid);
        else
            fprintf(stderr, COL_FG_RED "\r%s with pid %d exited abnormally\n" COL_RST, procName, pid);

        // delete it from the list
        if(proc.pid >= 0)
            procList = delete(procList, proc);

        printf("%s", makePrompt());
        fflush(stdout);
    }
}

void ctrlcHandler(int sig) {
    printf("\n%s", makePrompt());
    fflush(stdout);
}

void ctrlzHandler(int sig) {
    if(fgPid > 0) {
        kill(fgPid, SIGTSTP);

        printf("Suspended\t%s [%d]\n", getFullCommand(fgCommand), fgPid);

        Process p;
        p.pid = fgPid;
        p.name = (char *) malloc(strlen(fgCommand.command) + 1);
        strcpy(p.name, getFullCommand(fgCommand));

        procList = insert(procList, p);
    } else {
        printf("\n%s", makePrompt());
        fflush(stdout);
    }
}
