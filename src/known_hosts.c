/**
 * Naive, slow and simple storage of nodeid and hostname
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <slash/slash.h>
//#include <slash/optparse.h>
//#include <slash/dflopt.h>

#define MAX_HOSTS 100
#define MAX_NAMELEN 50

struct host_s {
    int node;
    char name[MAX_NAMELEN];
} known_hosts[100];

void known_hosts_del(int host) {

    for (int i = 0; i < MAX_HOSTS; i++) {

        if (known_hosts[i].node == host) {
            known_hosts[i].node = 0;
        }

    }

}

void known_hosts_add(int addr, char * new_name) {

    known_hosts_del(addr);

    /* Search for empty slot */
    for (int i = 0; i < MAX_HOSTS; i++) {
        if (known_hosts[i].node == 0) {
            known_hosts[i].node = addr;
            strncpy(known_hosts[i].name, new_name, MAX_NAMELEN);
            break;
        }
    }

}

int known_hosts_get_name(int find_host, char * name, int buflen) {

    for (int i = 0; i < MAX_HOSTS; i++) {

        if (known_hosts[i].node == find_host) {
            strncpy(name, known_hosts[i].name, buflen);
            return 1;
        }

    }

    return 0;

}


int known_hosts_get_node(char * find_name) {

    if (find_name == NULL)
        return 0;

    for (int i = 0; i < MAX_HOSTS; i++) {

        if (strncmp(find_name, known_hosts[i].name, MAX_NAMELEN) == 0) {
            return known_hosts[i].node;
        }

    }

    return 0;

}
