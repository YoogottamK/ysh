#include "jobs.h"
#include "list.h"

void jobs();

void jobsHandler(Command c) {
    if(c.argc)
        fprintf(stderr, "Usage: jobs\n");
    else
        jobs();
}

void jobs() {
    int index = 1;
    Node * curr = procList;

    while(curr) {
        printf("[%d] %s %s %d\n", index, "test", curr->proc.name, curr->proc.pid);
        index++;
        curr = curr->next;
    }
}
