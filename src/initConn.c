#include "../lib/csp/include/csp/drivers/can_socketcan.h"
#include "../lib/csp/src/drivers/can/can_socketcan.c"
#include <csp/csp_rtable.h>

#define CAN_BITRATE  1000000
#define PARAM_LIST_DYNAMIC 1

int initCAN(csp_iface_t * iface){
    if(csp_can_socketcan_open_and_add_interface("can0", CSP_IF_CAN_DEFAULT_NAME, CAN_BITRATE, true, &iface) == CSP_ERR_NONE){return 0;}
    else {return -1;}
}

int initZMQ(csp_iface_t * iface){
    return 0;
}
