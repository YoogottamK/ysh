#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#include "ls.h"

void lsHandler(Command c) {
    lsOpts opts = { .a = 0, .l = 0 };

    if(!c.argc)
        ls(".", opts);
    else {
        for(int i = 0; i < c.argc; i++) {
        }
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
    else if(S_ISLNK(st.st_mode))
        perm[0] = 'l';

    perm[1] = !!(st.st_mode & S_IRUSR) ? 'r' : '-';
    perm[2] = !!(st.st_mode & S_IWUSR) ? 'w' : '-';
    perm[3] = !!(st.st_mode & S_IXUSR) ? 'x' : '-';
    perm[4] = !!(st.st_mode & S_IRGRP) ? 'r' : '-';
    perm[5] = !!(st.st_mode & S_IWGRP) ? 'w' : '-';
    perm[6] = !!(st.st_mode & S_IXGRP) ? 'x' : '-';
    perm[7] = !!(st.st_mode & S_IROTH) ? 'r' : '-';
    perm[8] = !!(st.st_mode & S_IWOTH) ? 'w' : '-';
    perm[9] = !!(st.st_mode & S_IXOTH) ? 'x' : '-';

    return perm;
}

void lsPrint(char * baseDir, char * path, bool l) {
    char * fileName = (char*) malloc(MAX_LEN);
    // initialize as null string
    fileName[0] = 0;

    strcpy(fileName, baseDir);
    strcat(fileName, "/");
    strcat(fileName, path);

    if(l) {
        struct stat st;
        if(stat(fileName, &st) < 0) {
            perror(path);
            return;
        }

        char * perm = getPerm(st),
             *time = (char*) malloc(MAX_LEN);

        ctime_r(&st.st_mtime, time);
        time[16] = 0;

        printf("%s %ld\t%ld\t%s %s\t%s %s\n",
                perm, st.st_nlink, st.st_size, getpwuid(st.st_uid)->pw_name,
                getgrgid(st.st_gid)->gr_name, time + 4, path);

        free(time);
        free(perm);
    } else {
        printf("%s ", fileName);
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

    for(int i = 0; i < nItems; i++) {
        lsPrint(path, namelist[i]->d_name, opts.l);
        free(namelist[i]);
    }

    free(namelist);

    if(!opts.l)
        printf("\n");
}
