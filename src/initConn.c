#include "initConn.h"

#define CAN_BITRATE  1000000
#define PARAM_LIST_DYNAMIC 1
#define CSP_HAVE_STDIO 1

int __start_vmem, __stop_vmem;

int initCAN(csp_iface_t * iface){
    if(csp_can_socketcan_open_and_add_interface("can0", CSP_IF_CAN_DEFAULT_NAME, CAN_BITRATE, true, &iface) == CSP_ERR_NONE){return 0;}
    else {return -1;}
}

int initZMQ(csp_iface_t * iface){
    uint8_t address = 255;
    static uint8_t server_address = 0;
    const char * zmq_device = NULL;
    const char * rtable = NULL;

    csp_print("Initialising CSP");

    /* Init CSP */
    csp_init();

    //NEED TO START ROUTER HERE
    //ROUTER START IS SPECIFIC TO YOUR ARCHITECTURE

    /* Add interface(s) */
    csp_iface_t * default_iface = iface;
    int error = csp_zmqhub_init(0, zmq_device, 0, &default_iface);
    if (error != CSP_ERR_NONE) {
        csp_print("failed to add ZMQ interface [%s], error: %d\n", zmq_device, error);
            exit(1);
    }

    if (rtable) {
        int error = csp_rtable_load(rtable);
        if (error < 1) {
            csp_print("csp_rtable_load(%s) failed, error: %d\n", rtable, error);
            exit(1);
        }
    } else if (default_iface) {
        csp_rtable_set(0, 0, default_iface, CSP_NO_VIA_ADDRESS);
    } else {
        /* no interfaces configured - run server and client in process, using loopback interface */
        server_address = address;
    }

    csp_print("Connection table\r\n");
    csp_conn_print_table();

    csp_print("Interfaces\r\n");
    csp_iflist_print();

    csp_print("Route table\r\n");
    csp_rtable_print();

    /* Start server thread */
    if ((server_address == 255) ||  /* no server address specified, I must be server */
        (default_iface == NULL)) {  /* no interfaces specified -> run server & client via loopback */
        //server_start();
    }

    if ((server_address != 255) ||  /* server address specified, I must be client */
        (default_iface == NULL)) {  /* no interfaces specified -> run server & client via loopback */
        //client_start();
    }
    return 0;
}