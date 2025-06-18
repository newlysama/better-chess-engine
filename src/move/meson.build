#- Source Files -#
move_src = files(
  'src/move_generator.cpp'
)

#- Include Dirs -#
incs = [inc_src]

#- Move Library -#
move_lib = library(
  'move',
  move_src,
  include_directories: incs,
  dependencies: [logging_dep]
)

#- Exportable Module -#
move_dep = declare_dependency(
  link_with: move_lib,
  include_directories: incs,
  dependencies: [logging_dep]
)
