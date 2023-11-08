// Author: Mansour Savadogo

#ifndef EPS_DRIVER_POWERBOARD_H
#define EPS_DRIVER_POWERBOARD_H

#include <math.h>
#include <pthread.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

#include <param/param.h>
#include <param/param_list.h>
#include <param/param_client.h>
#include <vmem/vmem.h>

#include <csp/csp.h>
#include <csp/drivers/can_socketcan.h>
#include <csp/csp_rtable.h>


// HARDWARE PARAMETERS SET AS DEFINE STATEMENTS
// THOSE SHOULD NEVER NEED TO CHANGE DURING RUNTIME
#define PCDU 3
#define CSP_VERSION 2
#define VERBOSE 0
#define TIMEOUT 100
#define ARRAY_SIZE 12
#define ARRAY_STEP 0
#define CAN_BITRATE  1000000
#define TIMEOUT 100
#define PARAMID_SERIAL0 31

extern int __start_vmem, __stop_vmem;

    int initEPS(); //0 on success, otherwise 1

    //PCDU Getters
    //These update the local parameter and return its address.
    uint16_t get_vbatt();
    uint16_t get_temp();
    uint32_t get_pavg(int channel); //Average power usage for all 12 channels
    uint32_t get_ch_ilim(int channel); //Current limit for all 12 channels
    uint8_t get_ch_protect(int channel); //Voltage limit for all 12 channels
    uint16_t get_pcur(int channel); //Current power usage for all 12 channels
    
	//PCDU Setters
    //Setters return 0 on successful communication
    //Return 1 if unsuccessful
	//Return 2 if invalid input
    int turn_off_delay(int time_ms, int channel);
    int turn_on_delay(int time_ms, int channel);
    int turn_off(int channel);
    int turn_on(int channel);
    int set_ch_protect(int channel);
    int resetWatchdog(int time_ms);

//Defining hardware setup for subsystems
//Again, those should never need to change during runtime
#define PI_CHANNEL 0
#define PERIPHERAL_CHANNEL_1 1
#define PERIPHERAL_CHANNEL_2 2
#define PERIPHERAL_CHANNEL_3 3
//define the others

#endif
