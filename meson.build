project('SpaceInventorDeviceDriver', 'c', subproject_dir: 'lib', default_options: [
        'buildtype=debug',
        'c_std=gnu11',
        'param:list_dynamic=true',
	'csp:have_rtable=true']) #compiler arguments

csp_dep = dependency('csp', fallback: ['csp', 'csp_dep'], required: true)
param_dep = dependency('param', fallback: ['param', 'param_dep'], required: true).as_link_whole()

SI_test_src = [
	'src/blinkLED.c', 'src/initConn.c',
	'src/serial.c'
]

Embed_test = executable('can_test', ['src/template.c', 'src/serial.c'],
	dependencies : [csp_dep, param_dep],
        link_args : ['-Wl,-Map=can_test.map', '-lm'],
        install : false)

SI_test = executable('can_list_test', SI_test_src,
	dependencies : [csp_dep, param_dep],
    link_args : ['-Wl,-Map=can_test.map', '-lm'],
    install : false)
