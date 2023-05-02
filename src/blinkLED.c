#include <param/param.h>
#include <param/param_list.h>
#include <param/param_client.h>
#include <csp/csp.h>

#define CH_ON_OFF 20
#define NODE 3
#define CSP_VERSION 2
#define VERBOSE 0

int main(){
	param_list_download(NODE, 100, CSP_VERSION, 0);
	param_t * ch = param_list_find_id(NODE, CH_ON_OFF);
	
	char buffer[64] __attribute__(( aligned(16))) = {};
	buffer [0] = '0';
	
	for(int i = 0; i < 12; i++){
	int error = param_push_single(ch, i, buffer, VERBOSE, NODE, 100, CSP_VERSION);
	if(error) { printf("Connection timed out.\n");}
	}
	return 0;
}
