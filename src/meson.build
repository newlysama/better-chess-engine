#----- src/meson.build -----#

#- Subdirs -#

subdir('logging/')
subdir('engine/')

if console == true
    subdir('ui/console/')
elif server == true
    subdir('server')
endif

if mode != 'test'
    subdir('main/')
endif