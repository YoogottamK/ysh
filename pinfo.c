#include <fcntl.h>

#include "pinfo.h"
#include "utils.h"

void pinfo(char * pid);

void pinfoHandler(Command c) {
    if(!c.argc)
        pinfo("self");
    else if(c.argc == 1)
        pinfo(c.args[0]);
    else
        fprintf(stderr, "pinfo: Too many arguments\n");
}

void pinfo(char * pid) {
    char * procDir = (char*) malloc(MAX_LEN),
         *buf = (char*) malloc(MAX_LEN);

    int fd;

    sprintf(procDir, "/proc/%s/", pid);

    // To get the state of process
    fd = openFile(procDir, "stat");
    read(fd, buf, MAX_LEN);

    if(fd < 0) {
        fprintf(stderr, "Process with pid %s not found\n", pid);
        return;
    }

    if(!strcmp(pid, "self")) {
        pid_t self = getpid();
        printf("pid: %d\n", self);
    } else {
        printf("pid: %s\n", pid);
    }

    char * status = getArg(buf, 3);

    printf("Process status: %s\n", status);

    close(fd);

    // To get the memory used by process
    fd = openFile(procDir, "statm");
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
