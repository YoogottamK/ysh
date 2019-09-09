#include "pcwd.h"

void pwd();

void pwdHandler(Command c) {
    if(c.argc)
        fprintf(stderr, "pwd: Too many arguments\n");
    else
        pwd();
}

void pwd() {
    char dir[MAX_LEN];
    if(!getcwd(dir, MAX_LEN))
        perror("Error in retrieving current dir");

    printf("%s\n", dir);
}
