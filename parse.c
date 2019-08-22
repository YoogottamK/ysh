#include "parse.h"

int count(char * str, char delim) {
    int c = 0;

    if(!str)
        return 0;

    if(!strlen(str)) return 0;

    for(int i = 0; str[i]; i++) {
        if(str[i] == delim) {
            c++;

            while(str[i] == delim) i++;
        }
    }

    return c + 1;
}

char * stripSpaces(char * str) {
    if(!str) return str;

    while(*str == ' ' || *str == '\t') str++;

    if(!*str) return str;

    char * end = str + strlen(str) - 1;

    while(end > str && (*end == ' ' || *end == '\t')) end--;

    end[1] = 0;

    return str;
}

Parsed parse(char * str) {
    Parsed parsed;
    char * stripped = stripSpaces(str);

    parsed.n = count(stripped, ';');

    // if command is empty, return
    if(parsed.n <= 0) return parsed;

    // allocate space for commands
    parsed.commands = (Command*) malloc(parsed.n * sizeof(Command));

    // array to store individual commands, to be tokenized later
    char ** commands = (char**) malloc(parsed.n * sizeof(char*)),
         *command,
         *argument = (char*) malloc(MAX_LEN);

    int c = 0;

    command = strtok(stripped, ";");
    // store it for now, will tokenize later
    if(command)
        commands[c++] = stripSpaces(command);

    while(command) {
        command = strtok(0, ";");

        if(command) commands[c++] = stripSpaces(command);
    }

    parsed.n = c;

    // For replacing '~' with actual homedir
    char * tildaSubstituted = (char*) malloc(MAX_LEN);
    tildaSubstituted[0] = 0;
    strcpy(tildaSubstituted, HOME);

    // string is broken into ';' seperated 'commands'.
    // Now we have to break each command into constituents
    for(int i = 0; i < c; i++) {
        int n = count(commands[i], ' ');

        // initialize it to ""
        argument[0] = 0;

        // if somehow n is 0, argc = -1 represents empty command
        parsed.commands[i].argc = n - 1;

        // empty command => ignore
        if(!n) continue;

        // allocate space for arguments
        if(n > 1)
            parsed.commands[i].args = (char**) malloc((n - 1) * sizeof(char*));
        else
            parsed.commands[i].args = 0;

        // the command is the first part of the space separated string
        char * tok;
        tok = strtok(commands[i], " ");

        parsed.commands[i].command = tok;

        int homeLen = strlen(HOME);

        // other parts are arguments
        if(parsed.commands[i].argc > 0) {
            int argc = 0;
            while(tok) {
                tok = strtok(0, " \t");

                // if tok becomes NULL now, just break
                // required, since it would fail at tok[0] == '~' check
                if(!tok) break;

                parsed.commands[i].args[argc] = (char*) malloc(MAX_LEN);
                parsed.commands[i].args[argc][0] = 0;

                if(tok[0] == '~') {
                    strcat(tildaSubstituted, tok + 1);
                    strcpy(parsed.commands[i].args[argc], tildaSubstituted);

                    tildaSubstituted[homeLen] = 0;
                } else {
                    strcpy(parsed.commands[i].args[argc], tok);
                }

                argc++;
            }
        }

        // if the last arg is "&"
        if(parsed.commands[i].argc > 0) {
            if(!strcmp(parsed.commands[i].args[parsed.commands[i].argc - 1], "&")) {
                parsed.commands[i].bg = 1;
                parsed.commands[i].argc--;
            }
        } else
            parsed.commands[i].bg = 0;
    }

    free(tildaSubstituted);
    return parsed;
}

void dump(Parsed p) {
    if(p.n > 0) {
        for(int i = 0; i < p.n; i++) {
            if(p.commands[i].args)
                for(int j = 0; j < p.commands[i].argc; j++)
                    free(p.commands[i].args[j]);

            free(p.commands[i].args);
        }

        free(p.commands);
    }
}
