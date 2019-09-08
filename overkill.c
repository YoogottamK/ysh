#include <signal.h>

#include "overkill.h"

void overkill();

void overkillHandler(Command c) {
    if(c.args)
        fprintf(stderr, "Usage: overkill\n");
    else
        overkill();
}

void overkill() {
    Process p = get(procList, 0);

    while(p.pid > 0) {
        procList = delete(procList, p);

        kill(p.pid, SIGKILL);

        p = get(procList, 0);
    }
}
