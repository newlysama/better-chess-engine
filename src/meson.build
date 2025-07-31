#----- src/meson.build -----#

#- Subdirs -#

subdir('logging/')
subdir('engine/')

if console == true
    subdir('ui/console/')
endif

if mode not in ['test', 'benchmark']
    subdir('main/')
endif