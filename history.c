#include "history.h"

char * getFullCommand(Command c) {
    char * str = (char*) malloc(MAX_LEN);
    str[0] = 0;

    if(!c.command)
        return 0;

    strcat(str, c.command);

    for(int i = 0; i < c.argc; i++) {
        if(c.args[i]) {
            strcat(str, " ");
            strcat(str, c.args[i]);
        }
    }

    return str;
}

void updateHistory(Command c) {
    char * command = getFullCommand(c);

    if(!strcmp(command, HISTORY[HISTORY_INDEX % 20]))
        return;

    HISTORY_INDEX++;

    if(HISTORY_INDEX >= 40)
        HISTORY_INDEX -= 20;

    HISTORY[HISTORY_INDEX % 20][0] = 0;
    strcpy(HISTORY[HISTORY_INDEX % 20], command);

    free(command);
}

void historyHandler(Command c) {
    if(!c.argc)
        history(10);
    else if(c.argc == 1) {
        long n = strtol(c.args[0], 0, 10);
        history(n);
    } else {
        printf("history: Too many arguments\n");
    }
}

void history(int n) {
    bool goBack = HISTORY_INDEX >= 20;
    int start, end = HISTORY_INDEX + 1;

    if(n > 20) {
        n = 20;
        printf("I am only storing the last 20 conmmands\n");
    }

    if(goBack)
        start = end - n;
    else
        start = (end - n >= 0) ? end - n : 0;

    for(int i = start; i < end; i++)
        printf("%s\n", HISTORY[i % 20]);
}
