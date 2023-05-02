#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/utsname.h>
#include <time.h>

//#include <slash/slash.h>
//#include <slash/dflopt.h>

#include <csp/csp.h>
#include <csp/csp_yaml.h>
#include <csp/csp_cmp.h>


#include <param/param.h>
#include <param/param_list.h>
#include <param/param_server.h>
#include <param/param_collector.h>
#include <param/param_queue.h>
#include <param/param_commands.h>
#include <param/param_scheduler.h>
#include <param/param_client.h>

#include <vmem/vmem_server.h>
#include <vmem/vmem_file.h>

//#include "prometheus.h"
//#include "param_sniffer.h"
//#include "known_hosts.h"

#include <param/param_string.h>

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <csp/arch/csp_time.h>
#include <param/param.h>
#include <param/param_list.h>

#ifndef EPS_POWER_DRIVER_H
#define EPS_POWER_DRIVER_H

#define EPS_VBATT_ID 402
#define EPS_TLM_PAVG_ID 421

    /*
    This is effectively just a fancy wrapper for Space Inventor Code.
    The serial protocol is handled by the MCP251x driver native to Linux.
    libcsp handles the packets.
    libparam handles changing the behavior of your remote devices
    */

    #ifdef __cplusplus //Make C libraries play nice with C++
    extern "C"{
    #endif 

    void initPowerBoard(); //Initializes functions used in driver
    static int cmd_pull(unsigned int node, unsigned int timeout); //pulls all params from node
    static int list_download(unsigned int node, unsigned int timeout); //pulls list from node
    static int csp_scan(); //scan for nodes on network
    void printParamFields(); //print the name of every parameter currently in your list. 
    uint16_t getBatVolt(); //Returns battery voltage
    void * getValue(int node, int id); //returns a pointer to param->addr
    void * getArray(int node, int id); //returns a pointer to param->addr
    int setValue(int node, int id, int offset, int newValue);
    //set param->addr [offset] to newValue then push param to node.

    //idk why these are for, I just kept them from csh
    void * onehz_task(void * param);
    void * vmem_server_task(void * param);
    void * router_task(void * param);
    void kiss_discard(char c, void * taskwoken);
    uint64_t clock_get_nsec(void);

    #ifdef __cplusplus
    }
    #endif
#endif
