#- Source Files -#
board_src = files(
  'state.cpp'
)

deps = []

# If we want to generate magics, need openmp
if generate_magics
  board_src += files('magics_generator.cpp')
  deps += openmp_dep
endif

#- Include Dirs -#
incs = [inc_src]

#- Board Library -#
board_lib = library(
  'board',
  board_src,
  include_directories: incs,
  dependencies: deps
)

#- Exportable Module -#
board_dep = declare_dependency(
  link_with: board_lib,
  include_directories: incs,
  dependencies: deps
)