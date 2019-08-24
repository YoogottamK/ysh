#include <time.h>

#include "nightswatch.h"
#include "utils.h"

#define NW_DRT "dirty"
#define NW_INT "interrupt"

void nightswatchHelp() {
    fprintf(stderr, "Usage: nightswatch [-n freq] (dirty|interrupt)\n");
}

void nightswatchHandler(Command c) {
    if(!c.argc) {
        // no such combo is possible
        nightswatchHelp();
    } else if(c.argc == 1) {
        if(strcmp(c.args[0], NW_DRT) && strcmp(c.args[0], NW_INT)) {
            fprintf(stderr, "nightswatch: unknown command %s\n", c.args[0]);

            // if only one arg, must be command
            nightswatchHelp();
        } else
            nightswatch(c.args[0], 5);
    } else if(c.argc == 3) {
        long freq = 0;
        int freqIndex = -1,
            commandIndex = -1;

        for(int i = 0; i < c.argc; i++) {
            if(!strcmp(c.args[i], "-n"))
                freqIndex = i + 1;
            else if(!strcmp(c.args[i], NW_DRT) || !strcmp(c.args[i], NW_INT))
                commandIndex = i;
        }

        if(freqIndex < 0 || freqIndex > 2 || commandIndex < 0) {
            nightswatchHelp();
        } else {
            freq = strtol(c.args[freqIndex], 0, 10);
            nightswatch(c.args[commandIndex], freq);
        }
    } else {
        // no such combo is possible
        nightswatchHelp();
    }
}

void nightswatch(char * command, int freq) {
    if(freq < 1)
        freq = 1;

    FILE * f;
    char * fileName,
         *line;
    int c = -1;

    if(!strcmp(command, NW_DRT)) {
        fileName = "/proc/meminfo";
        c = 0;
    } else if(!strcmp(command, NW_INT)) {
        fileName = "/proc/interrupts";
        c = 1;

        f = fopen(fileName, "r");

        // I had no time so ¯\_(ツ)_/¯
        line = getLineStartsWith(f, "            CPU0");
        printf("%s", line);
        free(line);

        fclose(f);
    } else {
        nightswatchHelp();
    }

    if(c < 0)
        return;

    bool quit = false;

    while(!quit) {
        time_t init = time(0),
               now;

        f = fopen(fileName, "r");

        if(!c) {
            line = getLineStartsWith(f, "Dirty:");
        } else {
            // I had no time so ¯\_(ツ)_/¯
            line = getLineStartsWith(f, "   1:");

            int i;
            for(i = 0; line[i] != 'I'; i++);

            line[i] = '\n';
            line[i + 1] = 0;

            for(i = 0; line[i] != ':'; i++)
                line[i] = ' ';
            line[i] = ' ';
        }

        printf("%s", line);
        free(line);

        fclose(f);

        while(1) {
            now = time(0);
            if(now - init >= freq)
                break;

            if(keyDown() && getchar() == 'q') {
                quit = true;
                break;
            }
        }
    }
}
