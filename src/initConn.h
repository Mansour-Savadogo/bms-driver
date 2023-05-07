#include "../lib/csp/include/csp/drivers/can_socketcan.h"
#include "../lib/csp/include/csp/interfaces/csp_if_zmqhub.h"
#include <csp/csp_rtable.h>
#include <csp/csp.h>

int initCAN(csp_iface_t * iface);
int initZMQ(csp_iface_t * iface);