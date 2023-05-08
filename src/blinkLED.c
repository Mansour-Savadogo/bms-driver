#include <stdio.h>
#include <param/param.h>
#include <param/param_list.h>
#include <param/param_client.h>
#include <csp/csp.h>
#include <vmem/vmem.h>
#include "initConn.h"
#include <stdlib.h>

#define CH_ON_OFF 20
#define NODE 3
#define CSP_VERSION 2
#define VERBOSE 0
#define TIMEOUT 100

int main(){
	csp_iface_t * interface = NULL; const char * rtable = NULL;
	int error = 0;

	//initCAN(interface);
	initZMQ(interface);
	
	error = param_list_download(NODE, TIMEOUT, CSP_VERSION, 0); //I like using param_list because it is simple, however defining parameters individually is better for performance
	if (error) { printf("Connection error occurred. Check if you are connected to the circuit. Terminating program.\n"); return 1;}
	
	printf("Downloaded list from remote node.\n");
	
	param_t * ch = param_list_find_id(NODE, CH_ON_OFF);
	if(ch){printf("Found remote parameter.\n");}
	else { printf("Specified parameter not found. Check if ID is correct. Terminating program.\n"); return 1;}
	
	char buffer[64] __attribute__(( aligned(16))) = {};
	
	for(int i = 0; i < 12; i++){
	buffer [0] = '0';
	error = param_push_single(ch, i, buffer, VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}
	else { printf("Channel %d turned off.\n", i);}
	sleep(1);

	buffer [0] = '1';
	error = param_push_single(ch, i, buffer, VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}
	else { printf("Channel %d turned on.\n", i);}
	}

	printf("about to stop connection\n");
	
	//csp_can_socketcan_stop(interface);
	//csp_close();

	return 0;
}
