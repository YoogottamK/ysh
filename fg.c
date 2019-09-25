#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdio.h>

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
    Process p = get(procList, job - 1);
    pid_t toFg = p.pid;

    setpgid(toFg, getpgid(0));

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(0, toFg);

    kill(toFg, SIGCONT);
    // TODO: error detection

    int status;
    waitpid(toFg, &status, WUNTRACED);

    tcsetpgrp(0, getpgid(0));

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    procList = delete(procList, p);
}
