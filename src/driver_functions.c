#include "driver_functions.h"

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef SCNd8
#define __SCN8(x) __INT8 __STRINGIFY(x)
#define SCNd8       __SCN8(d)
#define SCNu8       __SCN8(u)
#define SCNx8       __SCN8(x)
#endif


extern const char *version_string;

#define PROMPT_BAD		    "\x1b[0;38;5;231;48;5;31;1m csh \x1b[0;38;5;31;48;5;236;22m! \x1b[0m "
#define LINE_SIZE		    128
#define HISTORY_SIZE		2048

//check whether I even need those files
VMEM_DEFINE_FILE(col, "col", "colcnf.vmem", 120);
VMEM_DEFINE_FILE(commands, "cmd", "commands.vmem", 2048);
VMEM_DEFINE_FILE(schedule, "sch", "schedule.vmem", 2048);
VMEM_DEFINE_FILE(dummy, "dummy", "dummy.txt", 1000000);

uint64_t clock_get_nsec(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1E9 + ts.tv_nsec;
}

void kiss_discard(char c, void * taskwoken) {
	putchar(c);
}

void * router_task(void * param) {
	while(1) {
		csp_route_work();
	}
}

void * vmem_server_task(void * param) {
	vmem_server_loop(param);
	return NULL;
}


void * onehz_task(void * param) {
	while(1) {
		param_schedule_server_update();
		sleep(1);
	}
	return NULL;
}

static int csp_scan()
{
	for (int i = 0; i < 0xFF; i++) {
        //printf("\rtrying %d: ", i);
        if (csp_ping(i, 20, 0, CSP_O_NONE) >= 0) {
            printf("\nFound something on addr %d...\n", i);
            
            struct csp_cmp_message message;
            if (csp_cmp_ident(i, 100, &message) == CSP_ERR_NONE) {
                //printf("%s\n%s\n%s\n%s %s\n", message.ident.hostname, message.ident.model, message.ident.revision, message.ident.date, message.ident.time);
            }
        }
    }
    return 0;
}

static int list_download(unsigned int node, unsigned int timeout){
    //change this so it does not print to the screen
    unsigned int version = 2;
    param_list_download(node, timeout, version);
    return 0;
}

static int cmd_pull(unsigned int node, unsigned int timeout) {
	//printing to screen bad, slows you a ridiculous amount
	unsigned int server = node;
	char * include_mask_str = NULL;
	char * exclude_mask_str = NULL;
	int paramver = 2;
	uint32_t include_mask = 0xFFFFFFFF;
	uint32_t exclude_mask = PM_REMOTE | PM_HWREG;

	if (include_mask_str)
		include_mask = param_maskstr_to_mask(include_mask_str);
	if (exclude_mask_str)
	    exclude_mask = param_maskstr_to_mask(exclude_mask_str);
	int verbose = 0;
	if (param_pull_all(verbose, server, include_mask, exclude_mask, timeout, paramver)) {
		printf("No response\n");
		return -4;
	}
	return 0;
}

void printParamFields(){ //This is mostly for debugging
	list_download(3,10);
	cmd_pull(3,10);
	param_t * param = NULL;
	
	// Go through the list of parameters
	param_list_iterator i = {};
	while ((param = param_list_iterate(&i)) != NULL) {
		printf("id:%d node:%d array_size=%d name=%s unit=%s docstr=%s timestamp=%d\n",
		param->id, param->node, param->array_size, param->name, param->unit, param->docstr, param->timestamp);
		}
}

param_t *get(int node, int ID) { //Should just use param_list_find_id

	int paramver = 2;
	int server = 0;
	int offset = -1;
	int timeout = 10; //I should be passing this as an argument tbh
	param_t * param = NULL, * myVar = NULL;

	// Go through the list of parameters
	param_list_iterator i = {};
	while ((param = param_list_iterate(&i)) != NULL) {

		if(param->id == ID && param->node == node){myVar = param; continue;}
		//Check for ID match
		//I should really be adding a continue; statement in that if statement.

		// Node match
		if (param->node != node) {
			continue;
		}
		// Select destination, host overrides parameter node
		int dest = node;
		if (server > 0)
			dest = server;
		if (param_pull_single(param, offset, 0, dest, timeout, paramver) < 0) {
			printf("No response\n");
			//param_print_value(param, -1);
			//;
			continue;
		}

	}
	if(myVar){ return myVar;}
	else {return NULL;}
}

void initPowerBoard(){

 	int csp_version = 2;
	char * rtable = NULL;
	char * yamlname = "CDH-SuperNova/Source_Code/Hardware_Device_Layer/Power_Board/yaml/can.yaml"; //Define which yaml file you are using
	char * dirname = getenv("HOME");
	unsigned int dfl_addr = 0; //Set the Pi's default node to node 0


	//printf("Initalizing csp, yaml, libparam\n");
	srand(time(NULL));
	void serial_init(void);
	serial_init();
	static char hostname[100];
	gethostname(hostname, 100);
	static char domainname[100];
	int res = getdomainname(domainname, 100);
	(void) res;

	//Initialize CSP
	struct utsname info;
	uname(&info);
	csp_conf.hostname = info.nodename;
	csp_conf.model = info.version;
	csp_conf.revision = info.release;
	csp_conf.version = csp_version;
	csp_conf.dedup = CSP_DEDUP_OFF;
	csp_init();

	//Setup the yaml init so we don't have to mess with Linux drivers
	if (strlen(dirname)) {
		char buildpath[100];
		snprintf(buildpath, 100, "%s/%s", dirname, yamlname);
		csp_yaml_init(buildpath, &dfl_addr);
	} else {
		csp_yaml_init(yamlname, &dfl_addr);

	}

	csp_rdp_set_opt(3, 10000, 5000, 1, 2000, 2);
	//csp_rdp_set_opt(5, 10000, 5000, 1, 2000, 4);
	//csp_rdp_set_opt(10, 10000, 5000, 1, 2000, 8);
	//csp_rdp_set_opt(25, 10000, 5000, 1, 2000, 20);
	//csp_rdp_set_opt(40, 3000, 1000, 1, 250, 35);

#if (CSP_HAVE_STDIO)
	if (rtable && csp_rtable_check(rtable)) {
		int error = csp_rtable_load(rtable);
		if (error < 1) {
			printf("csp_rtable_load(%s) failed, error: %d\n", rtable, error);
		}
	}
#endif
	(void) rtable;

	csp_bind_callback(csp_service_handler, CSP_ANY);
	csp_bind_callback(param_serve, PARAM_PORT_SERVER);


	vmem_file_init(&vmem_dummy);

	static pthread_t router_handle;
	pthread_create(&router_handle, NULL, &router_task, NULL);

	static pthread_t vmem_server_handle;
	pthread_create(&vmem_server_handle, NULL, &vmem_server_task, NULL);

	static pthread_t onehz_handle;
	pthread_create(&onehz_handle, NULL, &onehz_task, NULL);

	vmem_file_init(&vmem_commands);
	param_command_server_init();
	param_schedule_server_init();
}

uint16_t getBatVolt(){
	param_t *ptr = param_list_find_id(3,EPS_VBATT_ID);
	if(ptr) {return * (uint16_t*) ptr->addr;}
	else {return -1;}
}

void * getValue(int node, int id){
	param_t *ptr = param_list_find_id(node, id);
	if(ptr) {return ptr->addr;}
	else{return NULL;}
}

void * getArray(int node, int id){
	//only returns the first value, needs an offset parameter
	//check if ``return param->addr + offset;`` works
	param_t *ptr = param_list_find_id(node, id);
	if(ptr) {return ptr->addr;}
	else{return NULL;}
}

int setValue(int node, int id, int offset, int newValue){
	//Generic setter for devices using libcsp/libparam
	//Moving the offset parameter lets you go through an array
	//node refers to which device you're talking about. Node 0 would be the Pi,
	//node 3 the PCDU, node 2 the battery, etc.

	int version = 2, server = 0;
	list_download(node, 100); cmd_pull(node, 100);

	param_t  *param = NULL;
	param = param_list_find_id(node, id);
	if(param == NULL){
		printf("Parameter %d not found.Connection timed out.\n", id);
		return -1;
	}

	char buffer[128] __attribute__((aligned(16))) = {};
	char a = '0'+newValue; // This is janky and not a good way to do this, I can already see it breaking in weird ways
	param_str_to_value(param->type, &a, buffer);
	//param_value_str(param, newValue, buffer, 128); //This is not great, does not work
	//It sets it to 49 ('1' in ASCII) instead of 0b1

	if(param->node == 0){
		param_set(param, 0, buffer);//For local parameters hard-code offset = 0
		return 0;
	}
	if( param_push_single(param, offset, buffer, 0, node, 100, version) < 0 ) {
	//If there is an error in pushing the parameter
	//The 0 is for non-verbose, 100 ms timeout
		printf("\nNo response from remote device.\n");
		return -2;
	}
	param_print(param, -1, NULL, 0, 2);
	
	return 0;
}
