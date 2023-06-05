#include "eps.h"

//Utility functions
void initEPS(){
	//Not sure if we need this chunk
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
	//while(interface == NULL) {interface = csp_can_socketcan_init("can0", CAN_BITRATE, 0);} //This is not a good way to handle not getting a connection, implement better behavior.
	
	csp_rdp_set_opt(3, 10000, 5000, 1, 2000, 2); //idk if we need this line
	
	// I don't think this chunk is needed, but I'm keeping it here just in case
	srand(time(NULL));
	void serial_init(void);
	serial_init();
}

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