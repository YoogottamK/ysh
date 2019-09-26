#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdio.h>

#include "fg.h"
#include "utils.h"

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

    fgCommand.command = p.name;
    fgCommand.args = 0;

    setpgid(toFg, getpgid(0));

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(0, toFg);

    kill(toFg, SIGCONT);
    // TODO: error detection

    fgPid = toFg;

    int status;
    waitpid(toFg, &status, WUNTRACED);

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

    procList = delete(procList, p);
}
