#include "shell.h"
#include "parse.h"
#include "prompt.h"

#include "ls.h"
#include "echo.h"
#include "pwd.h"
#include "cd.h"

void init() {
    // initialize home with current path
    if(!getcwd(HOME, MAX_LEN))
        perror("Error initializing ~");
}

void execCommand(Command c) {
    if(!c.command || c.argc < 0)
        return;

    const char * builtin[] = {
        "cd",
        "pwd",
        "echo",
        "ls",
    };

    int n = sizeof(builtin) / sizeof(builtin[0]),
        command = -1;

    for(int i = 0; i < n; i++) {
        if(!strcmp(builtin[i], c.command)) {
            command = i;
            break;
        }
    }
    lsOpts p;
    p.l = p.a = 1;

    // have to exec builtin
    switch(command) {
        case 0:
            cdHandler(c);
            break;
        case 1:
            pwdHandler(c);
            break;
        case 2:
            echoHandler(c);
            break;
        case 3:
            //  lsHandler(c);
            ls(c.args[0], p);
            break;
        default:
            printf("%s not yet implemented\n", c.command);
            break;
    }
}

void repl() {
    size_t bufSize = 0;
    char * inp = 0;
    ssize_t inpSize;

    // the L in REPL
    while(1) {
        makePrompt();

        // the R in REPL
        inpSize = getline(&inp, &bufSize, stdin);
        inp[inpSize - 1] = 0;

        if(inpSize <= 0)
            break;

        // the E in REPL
        Parsed parsed = parse(inp);
        for(int i = 0; i < parsed.n; i++) {
            execCommand(parsed.commands[i]);
        }
        dump(parsed);
    }

    printf("\n");
    free(inp);
}

int main() {
    init();
    repl();

    return 0;
}
