#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "utils.h"

#include "cd.h"
#include "echo.h"
#include "env.h"
#include "external.h"
#include "history.h"
#include "jobs.h"
#include "kjob.h"
#include "ls.h"
#include "nightswatch.h"
#include "overkill.h"
#include "parse.h"
#include "pcwd.h"
#include "pinfo.h"
#include "prompt.h"
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
        "overkill"
    };

    int n = sizeof(builtin) / sizeof(builtin[0]),
        command = -1,
        stdoutSave = -1,
        stdinSave = -1,
        inFd = -1,
        outFd = -1;

    for(int i = 0; i < n; i++) {
        if(!strcmp(builtin[i], c.command)) {
            command = i;
            break;
        }
    }

    if(DEBUG) {
        printf(COL_BG_CYN COL_FG_BLK "===DEBUG===" COL_RST "\n");

        printf("%s\n", c.command);

        printf("args: ");
        for(int i = 0; i < c.argc; i++)
            printf("'%s' ", c.args[i]);
        printf("\n");

        printf("inp: %s\n", c.inp ? c.inp : "STDIN");
        printf("out: %s\n", c.out ? c.out : "STDOUT");
        printf("append: %d\n", c.append);

        printf(COL_BG_CYN COL_FG_BLK "====END====" COL_RST "\n");
    }

    // check if input is stdin or something else
    if(c.inp) {
        stdinSave = dup(STDIN_FILENO);
        inFd = open(c.inp, O_RDONLY, 0644);

        if(inFd < 0)
            perror("Error opening input file");
        else
            dup2(inFd, STDIN_FILENO);
    }

    // check if output is stdout or something else
    if(c.out) {
        stdoutSave = dup(STDOUT_FILENO);

        if(c.append)
            outFd = open(c.out, O_CREAT | O_WRONLY | O_APPEND, 0644);
        else
            outFd = open(c.out, O_CREAT | O_WRONLY | O_TRUNC, 0644);

        if(outFd < 0)
            perror("Error opening output file");
        else
            dup2(outFd, STDOUT_FILENO);
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
        default:
            systemCommand(c);
            break;
    }

    // if input was something else, restore stuff back to normal
    if(stdinSave > 0) {
        dup2(stdinSave, 0);
        close(inFd);
        close(stdinSave);
    }

    // if output was something else, restore stuff back to normal
    if(stdoutSave > 0) {
        dup2(stdoutSave, 1);
        close(outFd);
        close(stdoutSave);
    }
}

void repl() {
    //  size_t bufSize = 0;
    char * inp = 0;
    //  ssize_t inpSize;

    // the L in REPL
    while(1) {
        char * prompt = makePrompt();
        inp = readline(prompt);
        free(prompt);

        if(upCount) {
            if(upCount > h.index) {
                fprintf(stderr, "Recall capacity exceeded\n");
                rl_on_new_line();
            } else {
                inp[0] = 0;
                strcpy(inp, h.history[(h.index - upCount) % 20]);
            }

            upCount = 0;
        }

        if(!inp)
            break;

        // the R in REPL
        //  inpSize = getline(&inp, &bufSize, stdin);
        //  inp[inpSize - 1] = 0;

        //  if(inpSize <= 0)
            //  break;

        // the E in REPL
        Parsed parsed = parse(inp);
        for(int i = 0; i < parsed.n; i++)
            for(int j = 0; j < parsed.piped[i].n; j++)
                execCommand(parsed.piped[i].commands[j]);

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
