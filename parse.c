#include "parse.h"

typedef struct Split {
    int n;
    char ** tokens;
} Split;

void escapeQuotes(char * str, char x, char y) {
    bool inSingleQuotes = 0,
         inDoubleQuotes = 0;

    for(int i = 0; str[i]; i++) {
        if(str[i] == '\'')
            inSingleQuotes ^= 1;
        else if(str[i] == '"')
            inDoubleQuotes ^= 1;

        if((inSingleQuotes || inDoubleQuotes) && str[i] == x)
            str[i] = y;
    }
}

int count(char * str, char delim) {
    int c = 0;

    if(!str || !strlen(str)) return 0;

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

    while(*str == ' ') str++;

    if(!*str) return str;

    char * end = str + strlen(str) - 1;

    while(end > str && (*end == ' ')) end--;

    end[1] = 0;

    return str;
}

void stripQuotes(char * str) {
    if(!str) return;

    int e = strlen(str) - 1;

    if(str[0] == str[e] && (str[0] == '\'' || str[0] == '\"')) {
        str[e] = 0;

        for(int i = 0; i < e; i++)
            str[i] = str[i + 1];
    }
}

void replaceTabsWithSpace(char * str) {
    if(!str) return;

    for(int i = 0; str[i]; i++)
        if(str[i] == '\t')
            str[i] = ' ';
}

void initCommand(Command * c) {
    c->bg = c->append = 0;
    c->argc = -1;
    c->args = 0;
    c->command = c->out = c->inp = 0;
}

Split split(char * str, char delim) {
    Split sp;
    sp.n = -1;

    char delimStr[2] = { delim, 0 };

    int n = count(str, delim),
        i = 0;

    if(n <= 0)
        return sp;

    sp.tokens = (char**) malloc(n * sizeof(char*));
    char * buf;

    buf = strtok(str, delimStr);

    while(buf) {
        sp.tokens[i++] = stripSpaces(buf);

        buf = strtok(0, delimStr);
    }

    sp.n = i;

    return sp;
}

/*
 * Splits the '|' seperated input into Command *
 */
Command * parseCommands(Split p) {
    Command * ret = (Command*) malloc(p.n * sizeof(Command));

    // '|' seperated commands
    for(int i = 0; i < p.n; i++) {
        initCommand(&ret[i]);

        if(!strlen(p.tokens[i])) continue;

        escapeQuotes(p.tokens[i], ' ', 1);

        Split command = split(p.tokens[i], ' ');

        ret[i].command = command.tokens[0];
        ret[i].bg = !strcmp(command.tokens[command.n - 1], "&");

        ret[i].args = (char**) malloc(command.n * sizeof(char*));

        int argc = 0;

        // space seperated args
        for(int j = 1; j < command.n; j++) {
            escapeQuotes(command.tokens[j], 1, ' ');
            stripQuotes(command.tokens[j]);

            if(!strcmp(command.tokens[j], "&")) continue;

            if(!strcmp(command.tokens[j], "<")) {
                j++;
                if(j == command.n) break;

                ret[i].inp = realloc(ret[i].inp, strlen(command.tokens[j]) + 1);
                ret[i].inp[0] = 0;

                escapeQuotes(command.tokens[j], 1, ' ');
                strcpy(ret[i].inp, command.tokens[j]);
            } else if(!strcmp(command.tokens[j], ">") ||
                    !strcmp(command.tokens[j], ">>")) {
                ret[i].append = command.tokens[j][1] == '>';

                j++;
                if(j == command.n) break;

                ret[i].out = realloc(ret[i].out, strlen(command.tokens[j]) + 1);
                ret[i].out[0] = 0;

                escapeQuotes(command.tokens[j], 1, ' ');
                strcpy(ret[i].out, command.tokens[j]);
            } else {
                bool tildeSub = command.tokens[j][0] == '~';
                ret[i].args[argc] = (char*) malloc(strlen(command.tokens[j]) + 1 + (tildeSub * strlen(HOME)));

                ret[i].args[argc][0] = 0;

                if(tildeSub)
                    strcpy(ret[i].args[argc], HOME);

                strcat(ret[i].args[argc], command.tokens[j]);

                argc++;
            }
        }

        ret[i].args = realloc(ret[i].args, argc * sizeof(char*));
        ret[i].argc = argc;
    }

    return ret;
}

/*
 * Splits the ';' seperated input into Piped *
 */
Piped * parsePiped(Split input) {
    Piped * ret = (Piped*) malloc(input.n * sizeof(Piped));

    for(int i = 0; i < input.n; i++) {
        Split piped = split(input.tokens[i], '|');

        ret[i].n = piped.n;
        ret[i].commands = parseCommands(piped);
    }

    return ret;
}

Parsed parse(char * str) {
    Parsed parsed;
    replaceTabsWithSpace(str);

    char * stripped = stripSpaces(str);

    // split the string delimited by ';'
    Split input = split(stripped, ';');

    parsed.n = input.n;

    if(parsed.n <= 0)
        return parsed;

    // allocate space for commands
    parsed.piped = parsePiped(input);

    return parsed;
}

/*
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
*/
