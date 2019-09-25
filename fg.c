#include <signal.h>
#include <sys/wait.h>

#include "fg.h"

void fg(int job);

void fgHandler(Command c) {
    if(c.argc != 1) {
        fprintf(stderr, "Usage: fg <job id>\n");
    } else {
        fg(atoi(c.args[0]));
    }
}

void fg(int job) {
    Process p = get(procList, job);
    pid_t toFg = p.pid;

    kill(toFg, SIGCONT);
    procList = delete(procList, p);

    int status;
    waitpid(-1, &status, WUNTRACED);
}
