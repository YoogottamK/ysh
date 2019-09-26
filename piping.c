#include "piping.h"
#include "utils.h"

void execPiped(Piped p) {

    if(p.n == 1) {
        execCommand(p.commands[0]);
        return;
    }

    for(int i = 0; i < p.n; i++) {
        if(p.commands[i].argc < 0) {
            fprintf(stderr, "One of the piped commands is invalid\n");
            return;
        }
    }

    int stdinSave = dup(STDIN_FILENO),
        stdoutSave = dup(STDOUT_FILENO);

    int ** pipes = (int **) malloc((p.n - 1) * sizeof(int *));
    for(int i = 0; i < p.n - 1; i++)
        pipes[i] = (int *) malloc(2 * sizeof(int));

    for(int i = 0; i < p.n; i++) {
        if(i < p.n - 1)
            pipe(pipes[i]);

        if(!i) {
            dup2(pipes[i][1], STDOUT_FILENO);

            execCommand(p.commands[i]);

            close(pipes[i][1]);
        } else if(i == p.n - 1) {
            dup2(pipes[i - 1][0], STDIN_FILENO);
            dup2(stdoutSave, STDOUT_FILENO);

            execCommand(p.commands[i]);

            close(pipes[i - 1][0]);
        } else {
            dup2(pipes[i - 1][0], STDIN_FILENO);
            dup2(pipes[i][1], STDOUT_FILENO);

            execCommand(p.commands[i]);

            close(pipes[i - 1][0]);
            close(pipes[i][1]);
        }
    }

    dup2(stdinSave, STDIN_FILENO);
}
