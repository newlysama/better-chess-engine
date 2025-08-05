#----- src/meson.build -----#

#- Subdirs -#

subdir('logging/')
subdir('engine/')

if console == true
    subdir('ui/console/')
endif

if mode != 'test'
    subdir('main/')
endif