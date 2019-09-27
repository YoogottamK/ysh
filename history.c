#include "history.h"

void history(int n);

void updateHistory(Piped * piped) {
    char * command = malloc(MAX_LEN);
    command[0] = 0;

    for(int i = 0; i < piped->n; i++) {
        strcat(command, getFullCommand(piped->commands[i]));

        if(i != piped->n - 1)
            strcat(command, " | ");
    }

    if(!strcmp(command, h.history[h.index % 20]))
        return;

    h.index++;

    if(h.index >= 40)
        h.index -= 20;

    h.history[h.index % 20][0] = 0;
    strcpy(h.history[h.index % 20], command);
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

int getUpArrowCount(char * str) {
    if(!str)
        return 0;

    int l = strlen(str),
        c = 0;

    for(int i = 0; i < l; i += 3) {
        if(i < l - 2 && str[i] == 27 && str[i + 1] == 91 && str[i + 2] == 65)
            c++;
        else
            continue;
    }

    return c - 1;
}
