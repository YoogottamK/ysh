#include "pcwd.h"

void pwdHandler(Command c) {
    if(c.argc)
        fprintf(stderr, "pwd: Too many arguments\n");
    else
        pwd();
}

void pwd() {
    char dir[MAX_LEN];
    getcwd(dir, MAX_LEN);

    printf("%s\n", dir);
}
