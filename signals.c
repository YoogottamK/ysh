#include <sys/wait.h>

#include "signals.h"
#include "prompt.h"

void bgProcessExit(int sig) {
    int status;

    pid_t pid = waitpid(-1, &status, WNOHANG);

    if(pid > 0) {
        int i;

        for(i = 0; i < PROC_LIST; i++)
            if(p[i].pid == pid)
                break;

        if(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
            printf("\r%s with pid %d exited normally\n", i == PROC_LIST ? "Process" : p[i].procName, pid);
        else
            printf("\r%s with pid %d exited abnormally\n", i == PROC_LIST ? "Process" : p[i].procName, pid);

        p[i].pid = -1;
        p[i].procName[0] = 0;

        makePrompt();
        fflush(stdout);
    }
}
