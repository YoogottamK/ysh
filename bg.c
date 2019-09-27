#include <signal.h>

#include "bg.h"

void bg(int job);

void bgHandler(Command c) {
    if(c.argc != 1)
        fprintf(stderr, "Usage: bg <job id>\n");
    else
        bg(atoi(c.args[0]));
}

void bg(int job) {
    Process p = get(procList, job - 1);
    pid_t toBg = p.pid;

    if(kill(toBg, SIGCONT))
        perror("bg");
}
