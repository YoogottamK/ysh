#include "prompt.h"

#include "utils.h"

char * makePrompt() {
    char * cwd = (char*) malloc(MAX_LEN);
    if(!getcwd(cwd, MAX_LEN))
        perror("Error in getting current dir");

    char * loginName = getenv("USER");
    char * dir = replaceWithTilda(cwd);

    char * hostname = (char*) malloc(MAX_LEN);
    if(gethostname(hostname, MAX_LEN) < 0)
        perror("Error in printing hostname");

    char * prompt = (char*) malloc(MAX_LEN);

    sprintf(prompt, COL_FG_GRN "<%s@%s:" COL_FG_BLU "%s" COL_FG_GRN "> " COL_RST,
            loginName, hostname, dir);

    free(cwd);
    free(hostname);
    free(dir);

    return prompt;
}
