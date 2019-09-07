#include <signal.h>

#include "kjob.h"
#include "list.h"

void kjob(int job, int sig);

void kjobHandler(Command c) {
    if(c.argc != 2)
        fprintf(stderr, "Usage: kjob <pid> <sig>\n");
    else
        kjob(atoi(c.args[0]), atoi(c.args[1]));
}

void kjob(int job, int sig) {
    // -1 to account for indexing
    Process p = get(procList, job - 1);

    if(p.pid < 0) {
        fprintf(stderr, "An error occured while sending signal to the job requested\n");
    } else {
        if(kill(p.pid, sig))
            perror("kjob");
    }
}
