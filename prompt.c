#include "prompt.h"

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

    if(offset > 0) {
        dir[0] = '~';
        strcpy(dir + 1, cwd + offset);
    } else
        strcpy(dir, cwd);

    return dir;
}

void makePrompt() {
    char * cwd = (char*) malloc(MAX_LEN);
    if(!getcwd(cwd, MAX_LEN))
        perror("Error in getting current dir");

    char * loginName = getenv("USER");
    char * dir = replaceWithTilda(cwd);

    char * hostname = (char*) malloc(MAX_LEN);
    gethostname(hostname, MAX_LEN);

    printf(COL_FG_GRN "<%s@%s:" COL_FG_BLU "%s" COL_FG_GRN "> " COL_RST, loginName, hostname, dir);

    free(cwd);
    free(hostname);
    free(dir);
}
