project('driver_test', 'cpp','c', subproject_dir: 'lib', default_options: [
	'buildtype=debug', 
	'c_std=gnu11', 
	'b_lto=false',
	'default_library=static',
	'csp:packet_padding_bytes=42', 
	'csp:buffer_count=1000',
	'csp:buffer_size=2048',
	'csp:conn_max=20',
	'csp:conn_rxqueue_len=1000',
	'csp:qfifo_len=1000',
	'csp:rdp_max_window=1000',
	'csp:port_max_bind=16',
	'param:have_fopen=true', 
	'param:slash=false', 
	'param:commands=true',
	'param:scheduler=false',
	'param:commands_client=true',
	'param:scheduler_client=true',
	'param:collector=false', 
	'param:list_dynamic=true']) #compiler arguments

add_global_arguments('-I../include', language: 'c')
add_global_arguments('-I../include', language: 'cpp')

csp_dep = dependency('csp', fallback: ['csp', 'csp_dep'], required: true)#slash_dep = dependency('slash', fallback: ['slash', 'slash_dep'], required: true).as_link_whole()
param_dep = dependency('param', fallback: ['param', 'param_dep'], required: true).as_link_whole()

driver_test_sources = [
	'src/blinkLED.c',
	#'src/driver_functions.c',
	'src/serial.c',
	'src/known_hosts.c',
]

driver_test = executable('driver_test', driver_test_sources,
	dependencies : [csp_dep, param_dep],
	link_args : ['-Wl,-Map=driver_test.map', '-lm'],
	install : false,
)

#install_data('yaml/caninit', install_dir : get_option('bindir'))
