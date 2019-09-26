#include "redirect.h"

void redirectBegin(Command c) {
    stdoutSave = -1;
    stdinSave = -1;
    inFd = -1;
    outFd = -1;

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

}

void redirectRestore() {
    // if input was something else, restore stuff back to normal
    if(stdinSave > 0) {
        dup2(stdinSave, STDIN_FILENO);
        close(inFd);
        close(stdinSave);
    }

    // if output was something else, restore stuff back to normal
    if(stdoutSave > 0) {
        dup2(stdoutSave, STDOUT_FILENO);
        close(outFd);
        close(stdoutSave);
    }

}
