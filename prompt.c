#include "prompt.h"

#include "utils.h"

#define COL_BEG "\001"
#define COL_END "\002"

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

    sprintf(prompt, COL_BEG COL_FG_GRN COL_END "<%s@%s:" COL_BEG COL_FG_BLU COL_END
            "%s" COL_BEG COL_FG_GRN COL_END "> " COL_BEG COL_RST COL_END,
            loginName, hostname, dir);

    free(cwd);
    free(hostname);
    free(dir);

    return prompt;
}
