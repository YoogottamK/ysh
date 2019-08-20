#include "echo.h"

void echoHandler(Command c) {
    echo(c.args, c.argc);
}

void echo(char ** args, int n) {
    if(!args) {
        printf("\n");
        return;
    }

    for(int i = 0; i < n; i++)
        if(args[i]) printf("%s ", args[i]);

    printf("\n");
}
