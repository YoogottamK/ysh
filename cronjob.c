#include "cronjob.h"
#include "utils.h"

void cronjob(Command c, int f, int t);

void cronjobHandler(Command c) {
    if(c.argc != 6) {
        fprintf(stderr, "Usage: cronjob -c <cmd> -t <time> -p <period>\n");
    } else {
        char * cmd = 0;
        int t = -1,
            f = -1;

        for(int i = 0; i < 6; i++) {
            if(!(i & 1)) {
                if(!strcmp(c.args[i], "-p")) {
                    i++;
                    f = atoi(c.args[i]);
                } else if(!strcmp(c.args[i], "-t")) {
                    i++;
                    t = atoi(c.args[i]);
                } else if(!strcmp(c.args[i], "-c")) {
                    i++;
                    cmd = c.args[i];
                }
            }
        }

        if(!cmd || t < 0 || f < 0) {
            fprintf(stderr, "Usage: cronjob -c <cmd> -t <time> -p <period>\n");
        } else {
            Command command;
            command.append = 0;
            command.argc = 0;
            command.args = 0;
            command.bg = 0;
            command.command = cmd;
            command.inp = command.out = 0;

            cronjob(command, f, t);
        }
    }
}

void cronjob(Command c, int f, int t) {
    execCommand(c);
}
