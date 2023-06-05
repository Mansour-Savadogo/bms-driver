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

long int _serial0;
int __start_vmem, __stop_vmem;

// Now defining parameters. Again, those should never need to change during runtime.
// Just check that they were set up correctly here. CSH is your friend.

// Telemetry parameters
static uint32_t _tlm_pavg[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(421, tlm_pavg, PCDU, PARAM_TYPE_UINT32, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_tlm_pavg, "");
static uint16_t _temp0; PARAM_DEFINE_REMOTE_DYNAMIC(400, temp0, PCDU, PARAM_TYPE_UINT16, 1, ARRAY_STEP, PM_CONF, &_temp0, "");
static uint16_t _vbatt; PARAM_DEFINE_REMOTE_DYNAMIC(402, vbatt, PCDU, PARAM_TYPE_UINT16, 1, ARRAY_STEP, PM_CONF, &_vbatt, "");
static uint16_t _tlm_pcur[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(420, tlm_pcur, PCDU, PARAM_TYPE_UINT16, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_tlm_pcur, "");

//System control parameters
//Double check the data types, they're on the datasheet
static uint8_t _ch_on[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(20, ch_on, PCDU, PARAM_TYPE_UINT8, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_on, "");
static uint8_t _ch_protect[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(201, ch_protect, PCDU, PARAM_TYPE_UINT8, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_protect, "");
static uint32_t _ch_ilim[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(202, ch_ilim, PCDU, PARAM_TYPE_UINT32, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_ilim, "");
//
static uint32_t _dfl_on_in[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(236, dfl_on_in, PCDU, PARAM_TYPE_UINT32, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_dfl_on_in, "");
static uint32_t _dfl_off_in[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(237, dfl_off_in, PCDU, PARAM_TYPE_UINT32, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_dfl_off_in, "");


//Other parameters
PARAM_DEFINE_STATIC_RAM(PARAMID_SERIAL0, serial0, PARAM_TYPE_INT32, -1, 0, PM_HWREG, NULL, "", &_serial0, NULL);


// TO POWERCYCLE, SET POWERCYCLE PARAMETER TO 123

    void initEPS();

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
#define GPS_CHANNEL 1
#define CUBEADCS_CHANNEL 2
#define HAFX1_CHANNEL 3
//define the others

#endif