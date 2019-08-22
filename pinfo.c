#include <fcntl.h>

#include "pinfo.h"
#include "prompt.h"

void pinfoHandler(Command c) {
    if(!c.argc)
        pinfo("self");
    else if(c.argc == 1)
        pinfo(c.args[0]);
    else
        printf("pinfo: Too many arguments\n");
}

char * getArg(char * buf, int n) {
    if(n <= 0)
        return 0;

    if(n == 1)
        return strtok(buf, " ");

    strtok(buf, " ");

    for(int i = 0; i < n - 2; i++)
        strtok(0, " ");

    return strtok(0, " ");
}

int openProc(char * dir, char * file) {
    char * procFile = (char*) malloc(MAX_LEN);
    procFile[0] = 0;

    strcat(procFile, dir);
    strcat(procFile, file);

    int fd = open(procFile, O_RDONLY);

    free(procFile);

    return fd;
}

void pinfo(char * pid) {
    char * procDir = (char*) malloc(MAX_LEN),
         *buf = (char*) malloc(MAX_LEN);

    int fd;

    sprintf(procDir, "/proc/%s/", pid);

    if(!strcmp(pid, "self")) {
        pid_t self = getpid();
        printf("pid: %d\n", self);
    } else {
        printf("pid: %s\n", pid);
    }

    // To get the state of process
    fd = openProc(procDir, "stat");
    read(fd, buf, MAX_LEN);

    char * status = getArg(buf, 3);

    printf("Process status: %s\n", status);

    close(fd);

    // To get the memory used by process
    fd = openProc(procDir, "statm");
    read(fd, buf, MAX_LEN);

    char * mem = getArg(buf, 1);
    printf("Memory: %s\n", mem);

    close(fd);

    // To get the name of the process
    strcat(procDir, "exe");

    int l = readlink(procDir, buf, MAX_LEN);
    buf[l] = 0;

    char * path = replaceWithTilda(buf);

    printf("Executable path: %s\n", path);

    free(path);
    free(buf);
    free(procDir);
}
