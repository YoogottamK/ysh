#include <sys/types.h>
#include <stdbool.h>

#ifndef __LIST_H
#define __LIST_H

typedef struct Process {
    pid_t pid;
    char * name;
} Process;

typedef struct Node {
    struct Node * next;
     Process proc;
} Node;

/*
 * inserts a node to the list
 * returns the new root
 */
Node * insert(Node * root, Process p);

/*
 * deletes the node from the list
 *  whose pid matches with the one given
 * returns the new root
 */
Node * delete(Node * root, Process p);

/*
 * Returns the object if it exists in the list
 *  or else it returns an invalid process
 */
Process find(Node * root, Process p);

#endif // __LIST_H
