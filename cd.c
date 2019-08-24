#include "cd.h"

void cdHandler(Command c) {
    if(!c.argc) {
        cd(0);
    } else if(c.argc > 1) {
        fprintf(stderr, "cd: Too many arguments\n");
    } else {
        cd(c.args[0]);
    }
}

void cd(const char * path) {
    if(!path || !strlen(path)) {
        if(chdir(HOME))
            perror(path);
    } else {
        if(chdir(path))
            perror(path);
    }
}
