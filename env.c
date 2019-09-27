#include "env.h"

void setEnv(const char * name, const char * val);
void unsetEnv(const char * name);

void setEnvHandler(Command c) {
    if(c.argc == 1)
        setEnv(c.args[0], "");
    else if(c.argc == 2)
        setEnv(c.args[0], c.args[1]);
    else
        fprintf(stderr, "Usage: setenv NAME [VALUE]\n");
}

void unsetEnvHandler(Command c) {
    if(c.argc == 1)
        unsetEnv(c.args[0]);
    else
        fprintf(stderr, "Usage: unsetenv NAME\n");
}

void setEnv(const char * name, const char * val) {
    if(!name || !val)
        return;

    if(setenv(name, val, 1))
        perror("setenv");
}

void unsetEnv(const char * name) {
    if(!name)
        return;

    if(unsetenv(name))
        perror("unsetenv");
}
