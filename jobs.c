#include "jobs.h"
#include "list.h"
#include "utils.h"

void jobs();

void jobsHandler(Command c) {
    if(c.argc)
        fprintf(stderr, "Usage: jobs\n");
    else
        jobs();
}

void jobs() {
    int index = 1, fd;
    Node * curr = procList;

    char * buf = (char*) malloc(MAX_LEN);

    while(curr) {
        sprintf(buf, "/proc/%d/stat", curr->proc.pid);
        fd = open(buf, O_RDONLY);

        if(fd < 0) {
            fprintf(stderr, "Couldn't find relevant files for pid %d\n", curr->proc.pid);
            perror("jobs");
            continue;
        }

        read(fd, buf, MAX_LEN);

        char * status,
             procStat = getArg(buf, 3)[0];

        if(procStat == 'T')
            status = "Stopped";
        else if(procStat == 'Z')
            status = "Zombie";
        else
            status = "Running";

        printf("[%d] %s %s [%d]\n", index, status, curr->proc.name, curr->proc.pid);
        index++;
        curr = curr->next;

        close(fd);
    }

    free(buf);
}
