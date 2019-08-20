#include "prompt.h"

char * getDir() {
    char * cwd = (char*) malloc(MAX_LEN);
    if(!getcwd(cwd, MAX_LEN))
        perror("Error in getting current dir");

    unsigned long l = strlen(cwd);
    int offset = -1;

    // if the length of cwd >= len(HOME), might need to replace it with '~'
    if(l >= strlen(HOME)) {
        // if it starts with HOME, have to replace it with '~'
        if(!strncmp(cwd, HOME, l))
            offset = l;
    }

    char * dir = (char*) malloc(MAX_LEN);
    if(offset > 0) {
        dir[0] = '~';
        strcpy(dir + 1, cwd + l);
    } else
        strcpy(dir, cwd);

    free(cwd);
    return dir;
}

void makePrompt() {
    char * loginName = getlogin();
    char * dir = getDir();

    char * hostname = (char*) malloc(MAX_LEN);
    gethostname(hostname, MAX_LEN);

    printf(COL_FG_GRN "<%s@%s:" COL_FG_BLU "%s" COL_FG_GRN "> " COL_RST, loginName, hostname, dir);

    free(hostname);
    free(dir);
}
