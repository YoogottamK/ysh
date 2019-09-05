#include <termios.h>
#include <sys/ioctl.h>

#include "utils.h"

#include "cd.h"
#include "echo.h"
#include "env.h"
#include "history.h"
#include "ls.h"
#include "nightswatch.h"
#include "parse.h"
#include "pinfo.h"
#include "prompt.h"
#include "pcwd.h"
#include "external.h"

void init() {
    // clears the screen
    printf("\e[1;1H\e[2J");

    // initialize home with current path
    ssize_t len = readlink("/proc/self/exe", HOME, MAX_LEN);
    int i;

    for(i = len; HOME[i] != '/'; i--);
    HOME[i] = 0;

    // initialize procList as an empty list
    procList = 0;

    // restore history
    FILE * histfile = fopen(".ysh_history", "rb");

    if(!histfile)
        h.index = -1;
    else {
        fread(&h, sizeof(h), 1, histfile);
        fclose(histfile);
    }
}

void teardown() {
    FILE * histfile = fopen(".ysh_history", "wb");

    fwrite(&h, sizeof(h), 1, histfile);

    fclose(histfile);
}

void execCommand(Command c) {
    if(!c.command || c.argc < 0)
        return;

    const char * builtin[] = {
        "cd",
        "pwd",
        "echo",
        "ls",
        "pinfo",
        "history",
        "nightswatch",
        "setenv",
        "unsetenv",
        "exit"
    };

    int n = sizeof(builtin) / sizeof(builtin[0]),
        command = -1;

    for(int i = 0; i < n; i++) {
        if(!strcmp(builtin[i], c.command)) {
            command = i;
            break;
        }
    }

    updateHistory(c);

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
            lsHandler(c);
            break;
        case 4:
            pinfoHandler(c);
            break;
        case 5:
            historyHandler(c);
            break;
        case 6:
            nightswatchHandler(c);
            break;
        case 7:
            setEnvHandler(c);
            break;
        case 8:
            unsetEnvHandler(c);
            break;
        case 9:
            teardown();
            exit(0);
        default:
            systemCommand(c);
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
        for(int i = 0; i < parsed.n; i++)
            execCommand(parsed.commands[i]);

        dump(parsed);
    }

    printf("\n");
    free(inp);
}

char * replaceWithTilda(char * cwd) {
    unsigned long l = strlen(cwd),
                  lenHome = strlen(HOME);
    int offset = -1;

    // if the length of cwd >= len(HOME), might need to replace it with '~'
    if(l >= lenHome)
        // if it starts with HOME, have to replace it with '~'
        if(!strncmp(cwd, HOME, lenHome))
            offset = lenHome;

    char * dir = (char*) malloc(MAX_LEN);

    if(cwd[offset] && cwd[offset] != '/') {
        strcpy(dir, cwd);
        return dir;
    }

    if(offset > 0) {
        dir[0] = '~';
        strcpy(dir + 1, cwd + offset);
    } else
        strcpy(dir, cwd);

    return dir;
}

char * getArg(char * buf, int n) {
    if(n <= 0)
        return 0;

    if(n == 1)
        return strtok(buf, " ");

    strtok(buf, " ");

    for(int i = 0; i < n - 2; i++)
        strtok(0, " ");

    return strtok(0, " ");
}

int openFile(char * dir, char * file) {
    char * procFile = (char*) malloc(MAX_LEN);
    procFile[0] = 0;

    strcat(procFile, dir);
    strcat(procFile, file);

    int fd = open(procFile, O_RDONLY);

    free(procFile);

    return fd;
}

bool keyDown() {
    struct termios oldAttrs, newAttrs;
    int bytesWaiting;

    // get props
    tcgetattr(STDIN_FILENO, &oldAttrs);

    newAttrs = oldAttrs;

    // disable canonical mode and don't print
    newAttrs.c_lflag &= ~(ICANON | ECHO);

    // set new props
    tcsetattr(STDIN_FILENO, TCSANOW, &newAttrs);

    // check if some input is waiting
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);

    // reset params
    tcsetattr(STDIN_FILENO, TCSANOW, &oldAttrs);

    return bytesWaiting > 0;
}

char * getLineStartsWith(FILE * f, char * beg) {
    char * line = (char*) malloc(MAX_LEN);
    unsigned long l = strlen(beg);

    while(fgets(line, MAX_LEN, f)) {
        if(!strncmp(beg, line, l))
            break;
    }

    return line;
}
