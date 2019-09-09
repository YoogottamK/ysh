#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include "ls.h"

void ls(char * path, lsOpts opts);

unsigned long getSize(char * path, char * file) {
    char * fileName = (char*) malloc(MAX_LEN);
    fileName[0] = 0;

    strcpy(fileName, path);
    strcat(fileName, "/");
    strcat(fileName, file);

    struct stat st;
    lstat(fileName, &st);

    unsigned long size = st.st_blocks;

    free(fileName);

    return size;
}

void lsHandler(Command c) {
    lsOpts opts = { .a = 0, .l = 0 };

    if(!c.argc)
        ls(".", opts);
    else {
        int * skipOptsIndex = (int*) malloc(sizeof(int) * c.argc),
            nSkip = 0, skipIndex = 0;

        for(int i = 0; i < c.argc; i++) {
            if(c.args[i] && c.args[i][0] == '-') {
                skipOptsIndex[nSkip++] = i;

                for(int j = 1; c.args[i][j]; j++) {
                    switch(c.args[i][j]) {
                        case 'a':
                            opts.a = 1;
                            break;
                        case 'l':
                            opts.l = 1;
                            break;
                        default:
                            printf("ls: Unkown option -%c\n", c.args[i][j]);
                            return;
                    }
                }
            }
        }

        if(c.argc == nSkip) {
            ls(".", opts);
        } else {
            for(int i = 0; i < c.argc; i++) {
                if(skipIndex < nSkip && i == skipOptsIndex[skipIndex]) {
                    skipIndex++;
                    continue;
                }

                printf(COL_FG_YLW "%s: \n" COL_RST, c.args[i]);
                ls(c.args[i], opts);
            }
        }

        free(skipOptsIndex);
    }
}

int noHiddenFilter(const struct dirent *d) {
    return d->d_name[0] != '.';
}

char * getPerm(struct stat st) {
    char * perm = (char*) malloc(11);
    perm[10] = 0;

    perm[0] = '-';
    if(S_ISDIR(st.st_mode))
        perm[0] = 'd';
    if(S_ISLNK(st.st_mode))
        perm[0] = 'l';
    if(S_ISCHR(st.st_mode))
        perm[0] = 'c';
    if(S_ISSOCK(st.st_mode))
        perm[0] = 's';

    int masks[] = {
        S_IRUSR,
        S_IWUSR,
        S_IXUSR,
        S_IRGRP,
        S_IWGRP,
        S_IXGRP,
        S_IROTH,
        S_IWOTH,
        S_IXOTH
    };

    char c[] = { 'r', 'w', 'x' };

    for(int i = 0; i < 9; i++)
        perm[i + 1] = !!(st.st_mode & masks[i]) ? c[i % 3] : '-';

    return perm;
}

void lsPrint(char * baseDir, char * path, bool l) {
    char * fileName = (char*) malloc(MAX_LEN);
    // initialize as null string
    fileName[0] = 0;

    strcpy(fileName, baseDir);
    strcat(fileName, "/");
    strcat(fileName, path);

    struct stat st;
    if(lstat(fileName, &st) < 0) {
        perror(path);
        return;
    }

    char * perm = getPerm(st);

    char * col;
    if(perm[0] == 'd')
        col = COL_FG_BLU;
    else if(perm[0] == 'l')
        col = COL_FG_CYN;
    else if(perm[3] == 'x')
        col = COL_FG_GRN;
    else
        col = "";

    if(l) {
        char * time = (char*) malloc(MAX_LEN);

        ctime_r(&st.st_mtime, time);
        time[16] = 0;

        printf("%s %3ld %10ld %8s %8s     %s %s %s\n" COL_RST,
                perm, st.st_nlink, st.st_size, getpwuid(st.st_uid)->pw_name,
                getgrgid(st.st_gid)->gr_name, time + 4,
                col, path);

        free(time);
        free(perm);
    } else {
        printf("%s%s    " COL_RST, col, path);
    }

    free(fileName);
}

void ls(char * path, lsOpts opts) {
    struct dirent **namelist;
    int nItems;

    if(opts.a)
        nItems = scandir(path, &namelist, NULL, alphasort);
    else
        nItems = scandir(path, &namelist, noHiddenFilter, alphasort);

    if (nItems == -1) {
        perror(path);
        return;
    }

    unsigned long size = 0;

    for(int i = 0; i < nItems; i++)
        size += getSize(path, namelist[i]->d_name);

    if(opts.l)
        // stat works on the block device
        // ls works on filesystem
        // they have different block sizes
        printf("total: %ld\n", size >> 1);

    for(int i = 0; i < nItems; i++)
        lsPrint(path, namelist[i]->d_name, opts.l);

    for(int i = 0; i < nItems; i++)
        free(namelist[i]);

    free(namelist);

    if(!opts.l)
        printf("\n");
}
