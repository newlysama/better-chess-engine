#- Multi threading dep for magics generation -#
openmp_dep = dependency('openmp', required: false)

#- Source Files -#
board_src = files(
  'board.cpp'
)

# If we want to generate magics, need openmp
mode = get_option('mode')
if mode == 'generate_magics'
  board_src += files('magics_generator.cpp')
  openmp_dep = dependency('openmp', required: true)
endif

#- Include Dirs -#
incs = [inc_src]

#- Board Library -#
board_lib = library(
  'board',
  board_src,
  include_directories: incs,
  dependencies: [ logging_dep, openmp_dep ]
)

#- Exportable Module -#
board_dep = declare_dependency(
  link_with: board_lib,
  include_directories: incs,
  dependencies: [ logging_dep, openmp_dep ]
)