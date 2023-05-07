#include <stdio.h>
#include <param/param.h>
#include <param/param_list.h>
#include <param/param_client.h>
#include <csp/csp.h>
#include "initConn.h"

#define CH_ON_OFF 20
#define NODE 3
#define CSP_VERSION 2
#define VERBOSE 0

int main(){
	csp_iface_t * interface = NULL;
	
	//initCAN(interface);
	initZMQ(interface);
	
	param_list_download(NODE, 100, CSP_VERSION, 0); //I like using param_list because it is simple, however defining parameters individually is better for performance
	param_t * ch = param_list_find_id(NODE, CH_ON_OFF);
	
	char buffer[64] __attribute__(( aligned(16))) = {};
	
	for(int i = 0; i < 12; i++){
	buffer [0] = '0';
	int error = param_push_single(ch, i, buffer, VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}

	sleep(1);

	buffer [0] = '1';
	int error = param_push_single(ch, i, buffer, VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}
	}

	csp_can_socketcan_stop(interface);
	return 0;
}
