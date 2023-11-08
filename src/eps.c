#include "eps.h"

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
static long int _serial0; PARAM_DEFINE_STATIC_RAM(PARAMID_SERIAL0, serial0, PARAM_TYPE_INT32, -1, 0, PM_HWREG, NULL, "", &_serial0, NULL);

// TO POWERCYCLE, SET POWERCYCLE PARAMETER TO 123

//Utility functions
int initEPS(){
	//Initialize CSP
	struct utsname info;
	uname(&info);
	csp_conf.hostname = info.nodename;
	csp_conf.model = info.version;
	csp_conf.revision = info.release;
	csp_conf.version = CSP_VERSION;
	csp_conf.dedup = CSP_DEDUP_OFF;
	csp_init();
	
	// Initialize CAN bus
	csp_iface_t * interface = csp_can_socketcan_init("can0", CAN_BITRATE, 0);
	if(interface == NULL) {
		return 1;
	}
	
	csp_rdp_set_opt(3, 10000, 5000, 1, 2000, 2);
	
	// I don't think this chunk is needed, but I'm keeping it here just in case
	//srand(time(NULL));
	//void serial_init(void);
	//serial_init();
	
	return 0;
}

//Technically not using serial but there's some dependency for it in libparam
void serial_init(void) {
    _serial0 = rand();
}

unsigned long int serial_get(void) {
    return _serial0;
}

//Setters
//The channel here is just a fancy way to say offset
//channel = 0 changes the first channel, up to channel = 11
int turn_off_delay(int time_ms, int channel){
    if((channel > 11) | (channel < 0)){return 2;} //Check for invalid channel #
	
	_dfl_off_in[channel] = time_ms;
	if(param_push_single(&dfl_off_in, channel, &_dfl_off_in[channel], VERBOSE, PCDU, TIMEOUT, CSP_VERSION))
		{return 1;} //Connection error
    
	return 0;
}
int turn_on_delay(int time_ms, int channel){
    if((channel > 11) | (channel < 0)){return 2;} //Check for invalid channel #
	
	_dfl_on_in[channel] = time_ms;
	if(param_push_single(&dfl_on_in, channel, &_dfl_on_in[channel], VERBOSE, PCDU, TIMEOUT, CSP_VERSION))
		{return 1;} //Connection error
    
	return 0;
}
int turn_off(int channel){
    if((channel > 11) | (channel < 0)){return 2;} //Check for invalid channel #
	
	_ch_on[channel] = 0;
	if(param_push_single(&ch_on, channel, &_ch_on[channel], VERBOSE, PCDU, TIMEOUT, CSP_VERSION))
		{return 1;} //Connection error
    
	return 0;
}
int turn_on(int channel){
    if((channel > 11) | (channel < 0)){return 2;} //Check for invalid channel #
	
	_ch_on[channel] = 0;
	if(param_push_single(&ch_on, channel, &_ch_on[channel], VERBOSE, PCDU, TIMEOUT, CSP_VERSION) )
		{return 1;} //Connection error
    
	return 0;
}
int set_ch_protect(int channel){
    if((channel > 11) | (channel < 0)){return 2;} //Check for invalid channel #
	
	_ch_protect[channel] = 0;
	if(param_push_single(&ch_protect, channel, &_ch_protect[channel], VERBOSE, PCDU, TIMEOUT, CSP_VERSION) )
		{return 1;} //Connection error
    
	return 0;
}
int resetWatchdog(int time_ms){
	//Naive implementation of a watchdog, just call this function over and over
	//Check if this actually works, I'm not sure it does.
	turn_off_delay (PI_CHANNEL, time_ms);
	turn_on_delay (PI_CHANNEL, time_ms + 1000);
	return 0;
}

/////Getters//////

uint16_t get_vbatt(){
	param_pull_single(&vbatt, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _vbatt;
}
uint16_t get_temp(){
	param_pull_single(&temp0, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _temp0;
}
uint32_t get_pavg(int channel){
  param_pull_single(&tlm_pavg, -1, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
  return _tlm_pavg[channel];
}
uint32_t get_ch_ilim(int channel){
	param_pull_single(&ch_ilim, -1, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _ch_ilim[channel];
}
uint8_t get_ch_protect(int channel){
    param_pull_single(&ch_ilim, -1, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _ch_ilim[channel];
}
uint16_t get_pcur(int channel){
    param_pull_single(&tlm_pcur, -1, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _tlm_pcur[channel];
}
