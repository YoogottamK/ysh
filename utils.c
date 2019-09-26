/*
 * This file contains functions that are used in multiple places
 *
 * Instead of redefining them everywhere, I defined them here
 *  and include this file wherever they are needed
 */

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "utils.h"

#include "bg.h"
#include "cd.h"
#include "cronjob.h"
#include "echo.h"
#include "env.h"
#include "external.h"
#include "fg.h"
#include "history.h"
#include "jobs.h"
#include "kjob.h"
#include "ls.h"
#include "nightswatch.h"
#include "overkill.h"
#include "parse.h"
#include "pcwd.h"
#include "pinfo.h"
#include "piping.h"
#include "prompt.h"
#include "redirect.h"
#include "signals.h"

#ifndef DEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif

void init() {
    // clears the screen
    printf("\e[1;1H\e[2J");

    // initialize up arrow counter
    upCount = 0;

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

    // init fg process's pid
    fgPid = -1;
    fgCommand.command = 0;

    // setup shell signal handlers
    signal(SIGINT, ctrlcHandler);
    signal(SIGTSTP, ctrlzHandler);
    signal(SIGCHLD, bgProcessExit);
}

void teardown() {
    printf("\n");

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
        "jobs",
        "kjob",
        "quit",
        "overkill",
        "fg",
        "bg",
        "cronjob"
    };

    int n = sizeof(builtin) / sizeof(builtin[0]),
        command = -1;

    for(int i = 0; i < n; i++) {
        if(!strcmp(builtin[i], c.command)) {
            command = i;
            break;
        }
    }

    if(DEBUG) {
        fprintf(stderr, COL_BG_CYN COL_FG_BLK "===DEBUG===" COL_RST "\n");

        fprintf(stderr, "%s\n", c.command);

        fprintf(stderr, "args: ");
        for(int i = 0; i < c.argc; i++)
            fprintf(stderr, "'%s' ", c.args[i]);
        fprintf(stderr, "\n");

        fprintf(stderr, "inp: %s\n", c.inp ? c.inp : "STDIN");
        fprintf(stderr, "out: %s\n", c.out ? c.out : "STDOUT");
        fprintf(stderr, "append: %d\n", c.append);

        fprintf(stderr, COL_BG_CYN COL_FG_BLK "====END====" COL_RST "\n");
    }

    redirectBegin(c);

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
            jobsHandler(c);
            break;
        case 10:
            kjobHandler(c);
            break;
        case 11:
            teardown();
            exit(0);
        case 12:
            overkillHandler(c);
            break;
        case 13:
            fgHandler(c);
            break;
        case 14:
            bgHandler(c);
            break;
        case 15:
            cronjobHandler(c);
            break;
        default:
            systemCommand(c);
            break;
    }

    redirectRestore();
}

void repl() {
    size_t bufSize = 0;
    char * inp = 0;
    ssize_t inpSize;

    // the L in REPL
    while(1) {
        char * prompt = makePrompt();

        if(upCount) {
            if(upCount > h.index) {
                fprintf(stderr, "Recall capacity exceeded\n");
            } else {
                inp[0] = 0;
                strcpy(inp, h.history[(h.index - upCount) % 20]);
            }

            upCount = 0;
        }

        // the R in REPL
        printf("%s", prompt);
        inpSize = getline(&inp, &bufSize, stdin);
        inp[inpSize - 1] = 0;

        if(inpSize <= 0)
            break;

        // the E in REPL
        Parsed parsed = parse(inp);

        if(DEBUG) {
            fprintf(stderr, COL_BG_YLW COL_FG_BLK "============" COL_RST "\n");
            for(int i = 0; i < parsed.n; i++) {
                for(int j = 0; j < parsed.piped[i].n; j++) {
                    Command c = parsed.piped[i].commands[j];

                    fprintf(stderr, COL_BG_YLW COL_FG_BLK "===COMMAND===" COL_RST "\n");

                    if(c.argc < 0) {
                        fprintf(stderr, "Something was wrong with this command\n");
                        continue;
                    }

                    fprintf(stderr, "%s\n", c.command);

                    fprintf(stderr, "args: ");
                    for(int i = 0; i < c.argc; i++)
                        fprintf(stderr, "'%s' ", c.args[i]);
                    fprintf(stderr, "\n");

                    fprintf(stderr, "inp: %s\n", c.inp ? c.inp : "STDIN");
                    fprintf(stderr, "out: %s\n", c.out ? c.out : "STDOUT");
                    fprintf(stderr, "append: %d\n", c.append);

                    fprintf(stderr, COL_BG_YLW COL_FG_BLK "======END======" COL_RST "\n");
                }
            }
            fprintf(stderr, COL_BG_YLW COL_FG_BLK "============" COL_RST "\n");
        }

        for(int i = 0; i < parsed.n; i++)
            execPiped(parsed.piped[i]);

        //  dump(parsed);
    }

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

int upHist(int c, int k) {
    upCount++;

    if(upCount <= h.index)
        printf("\r\33[2K%s%s", makePrompt(), h.history[(h.index - upCount) % 20]);

    return 0;
}
