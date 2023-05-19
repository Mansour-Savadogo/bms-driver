/*
Author: Mansour Savadogo
Date: May 17, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>

#include <param/param.h>
#include <param/param_list.h>
#include <param/param_client.h>
#include <vmem/vmem.h>

#include <csp/csp.h>
#include <csp/drivers/can_socketcan.h>
#include <csp/csp_rtable.h>


// HARDWARE PARAMETERS SET AS DEFINE STATEMENTS
// THOSE SHOULD NEVER NEED TO CHANGE DURING RUNTIME
#define NODE 3
#define CSP_VERSION 2
#define VERBOSE 0
#define TIMEOUT 100
#define ARRAY_SIZE 12
#define ARRAY_STEP 0
#define CAN_BITRATE  1000000
#define TIMEOUT 100

int __start_vmem, __stop_vmem;

// Telemetry parameters
static uint32_t _tlm_pavg[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(421, tlm_pavg, NODE, PARAM_TYPE_UINT32, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_tlm_pavg, "");
static uint16_t _temp0; PARAM_DEFINE_REMOTE_DYNAMIC(400, temp0, NODE, PARAM_TYPE_UINT16, 1, ARRAY_STEP, PM_CONF, &_temp0, "");
static uint16_t _vbatt; PARAM_DEFINE_REMOTE_DYNAMIC(402, vbatt, NODE, PARAM_TYPE_UINT16, 1, ARRAY_STEP, PM_CONF, &_vbatt, "");

//System control parameters
static uint8_t _ch_on[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(20, ch_on, NODE, PARAM_TYPE_UINT8, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_on, "");
static uint8_t _ch_protect[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(201, ch_protect, NODE, PARAM_TYPE_UINT8, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_protect, "");
static uint16_t _dfl_on_in[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(236, dfl_on_in, NODE, PARAM_TYPE_UINT16, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_dfl_on_in, "");
static uint16_t _dfl_off_in[ARRAY_SIZE]; PARAM_DEFINE_REMOTE_DYNAMIC(237, dfl_off_in, NODE, PARAM_TYPE_UINT16, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_dfl_off_in, "");

int main(){
	csp_iface_t * interface = csp_can_socketcan_init("can0", CAN_BITRATE, 0);
	int error = 0;

	if(interface == NULL){printf("Connection error occurred. Check if you are connected to the circuit.\n"); return 1;}

	/*
	//Do things here.
	
	//You can turn off channel 2 by doing:
	_ch_on[1] = 0; param_push_single(&ch_on, i, &_ch_on[1], VERBOSE, NODE, TIMEOUT, CSP_VERSION);
	
	//You can turn it back on with:
	_ch_on[1] = 1; param_push_single(&ch_on, i, &_ch_on[1], VERBOSE, NODE, TIMEOUT, CSP_VERSION);
	
	//Download telemetry one at a time:
	param_pull_single(&vbatt, 0, VERBOSE, NODE, TIMEOUT, CSP_VERSION); printf("GPS voltage is %d V", _vbatt[3]);
	*/

	printf("about to stop connection\n");

	return 0;
}
