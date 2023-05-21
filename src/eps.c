#include "eps.h"

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
	while(interface == NULL) {interface = csp_can_socketcan_init("can0", CAN_BITRATE, 0);} //This is not a good way to handle not getting a connection, implement better behavior.
	
	csp_rdp_set_opt(3, 10000, 5000, 1, 2000, 2); //idk if we need this line
	
	// I don't think this chunk is needed, but I'm keeping it here just in case
	srand(time(NULL));
	void serial_init(void);
	serial_init(); //*/
	
	//90% sure we don't need this
	/*
	#if (CSP_HAVE_STDIO)
	if (rtable && csp_rtable_check(rtable)) {
		int error = csp_rtable_load(rtable);
		if (error < 1) {
			printf("csp_rtable_load(%s) failed, error: %d\n", rtable, error);
		}
	}
	#endif
	*/
	//No idea for this one
	//csp_bind_callback(csp_service_handler, CSP_ANY);
	//csp_bind_callback(param_serve, PARAM_PORT_SERVER);
	//param_command_server_init();
	//param_schedule_server_init();
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
int setWatchdog(int time_ms){
	//Naive implementation of a watchdog
	//The Pi should ideally use this function once every hour or so. Or every whatever unit of time.
	//If not used after time_ms milliseconds, it will get powercycled.
	turn_off_delay (PI_CHANNEL, time_ms);
	turn_on_delay (PI_CHANNEL, time_ms + 1000);
	return 0;
}

///////////////
//Getters//////
///////////////

//I do not have the telemetry parameters defined yet so I commented out these functions. If you could check the hardware and define them, it would be epic.
/*
    int get_ch_ilim(int channel){
	param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _ch_ilim[channel];
    
	}
    
    int get_ch_protect(int channel){
    param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _ch_ilim[channel];
    }
    
    int get_pavg(int channel){
    param_pull_single(&pavg, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _pavg[channel];
    }
    
    int get_pcur(int channel){
    param_pull_single(&pcur, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
    return _pcur[channel];
    }
    
    int get_vcur(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
    
    int get_icur(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
    
    int get_fault(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
    
    int get_energy(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
    
    int get_vmin(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
    
    int get_vmax(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
    
    int get_imax(int channel){
        int arr;
		param_pull_single(&ch_ilim, 0, VERBOSE, PCDU, TIMEOUT, CSP_VERSION);
		for(int k = 0; k < 12; k++){
        arr.push_back(_ch_ilim[k]);
		}
		return arr;
    }
	
//*/