/*
Author: Mansour Savadogo
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
#define CH_ON_OFF 20
#define NODE 3
#define CSP_VERSION 2
#define VERBOSE 0
#define TIMEOUT 100
#define TYPE PARAM_TYPE_UINT8
#define ARRAY_SIZE 12
#define ARRAY_STEP 0
#define CAN_BITRATE  1000000

int __start_vmem, __stop_vmem;

static uint8_t _ch_on[ARRAY_SIZE];
PARAM_DEFINE_REMOTE_DYNAMIC(CH_ON_OFF, ch_on, NODE, TYPE, ARRAY_SIZE, ARRAY_STEP, PM_CONF, &_ch_on, "");

int main(){
	csp_iface_t * interface = csp_can_socketcan_init("can0", CAN_BITRATE, 0);
	int error = 0;

	if(interface == NULL){printf("Connection error occurred. Check if you are connected to the circuit.\n"); return 1;}

	for(int i = 0; i < 12; i++){
	
    _ch_on[i] = 0;
	error = param_push_single(&ch_on, i, &_ch_on[i], VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}
	else { printf("Channel %d turned off.\n", i);}
	
    sleep(10);

	_ch_on[i] = 1;
	error = param_push_single(&ch_on, i, &_ch_on[i], VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}
	else { printf("Channel %d turned on.\n", i);}
	}

	printf("about to stop connection\n");
	
    if(interface){ csp_can_socketcan_stop(interface); }

	return 0;
}
