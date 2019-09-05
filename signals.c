#include <sys/wait.h>

#include "signals.h"
#include "prompt.h"

void bgProcessExit(int sig) {
    int status;

    pid_t pid = waitpid(-1, &status, WNOHANG);

    if(pid > 0) {
        Process p = { .pid = pid, .name = 0 },
                proc;

        proc = find(procList, p);

        if(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
            fprintf(stderr, COL_FG_YLW "\r%s with pid %d exited normally\n" COL_RST, proc.pid < 0 ? "Process" : proc.name, pid);
        else
            fprintf(stderr, COL_FG_RED "\r%s with pid %d exited abnormally\n" COL_RST, proc.pid < 0 ? "Process" : proc.name, pid);

        // delete it from the list
        if(proc.pid >= 0)
            procList = delete(procList, proc);

        makePrompt();
        fflush(stdout);
    }
}

void stopBgProcess(int sig) {
    raise(SIGTSTP);
}
