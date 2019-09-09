#include "history.h"

void history(int n);

void updateHistory(Command c) {
    char * command = getFullCommand(c);

    if(!strcmp(command, h.history[h.index % 20]))
        return;

    h.index++;

    if(h.index >= 40)
        h.index -= 20;

    h.history[h.index % 20][0] = 0;
    strcpy(h.history[h.index % 20], command);

    free(command);
}

void historyHandler(Command c) {
    if(!c.argc)
        history(10);
    else if(c.argc == 1) {
        long n = strtol(c.args[0], 0, 10);
        history(n);
    } else {
        fprintf(stderr, "history: Too many arguments\n");
    }
}

void history(int n) {
    bool goBack = h.index >= 20;
    int start, end = h.index + 1;

    if(n > 20) {
        n = 20;
        fprintf(stderr, "I am only storing the last 20 conmmands\n");
    }

    if(goBack)
        start = end - n;
    else
        start = (end - n >= 0) ? end - n : 0;

    for(int i = start; i < end; i++)
        printf("%s\n", h.history[i % 20]);
}
