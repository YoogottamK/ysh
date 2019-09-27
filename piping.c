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

    // save stdin & stdout for restoring it later
    int stdinSave = dup(STDIN_FILENO),
        stdoutSave = dup(STDOUT_FILENO);

    // array of pipes, will be populated as and when required
    int ** pipes = (int **) malloc((p.n - 1) * sizeof(int *));
    for(int i = 0; i < p.n - 1; i++)
        pipes[i] = (int *) malloc(2 * sizeof(int));

    for(int i = 0; i < p.n; i++) {
        // create the pipe for this command
        if(i < p.n - 1)
            if(pipe(pipes[i]))
                perror("pipe");

        if(!i) {
            // first command, read from stdin
            // use this pipe's write end as stdout
            dup2(pipes[i][1], STDOUT_FILENO);

            execCommand(p.commands[i]);

            close(pipes[i][1]);
        } else if(i == p.n - 1) {
            // last command, read from pipe
            dup2(pipes[i - 1][0], STDIN_FILENO);

            // restore stdout
            dup2(stdoutSave, STDOUT_FILENO);

            execCommand(p.commands[i]);

            close(pipes[i - 1][0]);
        } else {
            // intermediate command, read from previous pipe's read end
            // write to this pipe's write end
            dup2(pipes[i - 1][0], STDIN_FILENO);
            dup2(pipes[i][1], STDOUT_FILENO);

            execCommand(p.commands[i]);

            close(pipes[i - 1][0]);
            close(pipes[i][1]);
        }
    }

    // restore stdin
    dup2(stdinSave, STDIN_FILENO);

    for(int i = 0; i < p.n - 1; i++)
        free(pipes[i]);
    free(pipes);
}
