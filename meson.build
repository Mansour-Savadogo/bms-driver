project('SpaceInventorDeviceDriver', 'c', 'cpp', subproject_dir: 'lib', default_options: [
        'buildtype=debug', #Obviously change this on final release
        'c_std=gnu11',
        'param:list_dynamic=true',
	'csp:have_rtable=true']) #compiler arguments

csp_dep = dependency('csp', fallback: ['csp', 'csp_dep'], required: true)
param_dep = dependency('param', fallback: ['param', 'param_dep'], required: true).as_link_whole()

Embed_test = executable('can_test', ['src/can_test.cpp', 'src/eps.c'],
	dependencies : [csp_dep, param_dep],
        link_args : ['-Wl,-Map=can_test.map', '-lm'],
        install : false)

cpp_test = executable('cpp_test', ['src/template.cpp'],
	dependencies : [csp_dep, param_dep],
        link_args : ['-Wl,-Map=can_test.map', '-lm'],
        install : false)
#This defines an executable.
#You could have one executable to get telemetry, one executable to change the device mode, etc. etc.
