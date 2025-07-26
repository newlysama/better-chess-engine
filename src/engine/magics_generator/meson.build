#- Source File -#
magics_generator_src = files(
  'magics_generator.cpp'
)

# Multi threading dep
openmp_dep = dependency(
  'openmp',
  required: true
)

#- Board Library -#
magics_generator_lib = library(
  'magics_generator',
  magics_generator_src,
  include_directories: [inc_src],
  dependencies: [openmp_dep, board_dep]
)

#- Exportable Module -#
magics_generator_dep = declare_dependency(
  link_with: magics_generator_lib,
  include_directories: [inc_src],
  dependencies: deps
)