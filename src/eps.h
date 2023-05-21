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

int __start_vmem, __stop_vmem;

// Now defining parameters. Again, those should never need to change during runtime.
// Just check that they were set up correctly here. CSH is your friend.

// Telemetry parameters
static uint32_t _tlm_pavg[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(421, tlm_pavg, PCDU, PARAM_TYPE_UINT32, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_tlm_pavg, "");
static uint16_t _temp0; PARAM_DEFINE_REMOTE_DYNAMIC(400, temp0, PCDU, PARAM_TYPE_UINT16, 1, ARRAY_STEP, PM_CONF, &_temp0, "");
static uint16_t _vbatt; PARAM_DEFINE_REMOTE_DYNAMIC(402, vbatt, PCDU, PARAM_TYPE_UINT16, 1, ARRAY_STEP, PM_CONF, &_vbatt, "");

//System control parameters
static uint8_t _ch_on[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(20, ch_on, PCDU, PARAM_TYPE_UINT8, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_on, "");
static uint8_t _ch_protect[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(201, ch_protect, PCDU, PARAM_TYPE_UINT8, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_protect, "");
static uint16_t _dfl_on_in[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(236, dfl_on_in, PCDU, PARAM_TYPE_UINT16, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_dfl_on_in, "");
static uint16_t _dfl_off_in[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(237, dfl_off_in, PCDU, PARAM_TYPE_UINT16, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_dfl_off_in, "");

// TO POWERCYCLE, SET POWERCYCLE PARAMETER TO 123

    void initEPS();

    //Getters
	/*
    int get_vbatt(int node);
    int get_temp(int node);
    //Array getters. Not implemented yet as of 08/24/202
    int get_ch_ilim(int channel);
    int get_ch_protect(int channel);
    int get_pavg(int channel);
    int get_pcur(int channel);
    int get_vcur(int channel);
    int get_icur(int channel);
    int get_fault(int channel);
    int get_energy(int channel);
    int get_vmin(int channel);
    int get_vmax(int channel);
    int get_imax(int channel);
	*/
    
	//Setters
    //Setters return 0 on successful communication
    //Return 1 if unsuccessful
	//Return 2 if invalid input
    int turn_off_delay(int time_ms, int channel);
    int turn_on_delay(int time_ms, int channel);
    int turn_off(int channel);
    int turn_on(int channel);
    int set_ch_protect(int channel);
    int setWatchdog(int time_ms);

//Defining hardware setup for subsystems
//Again, those should never need to change during runtime
#define PI_CHANNEL 0
#define GPS_CHANNEL
#define CUBEADCS_CHANNEL
#define HAFX1_CHANNEL
//define the others

#endif