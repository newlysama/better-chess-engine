#----- src/engine/board/meson.build -----#

#- Source File -#
board_src = files(
  'state.cpp'
)

#- Board Library -#
board_lib = library(
  'board',
  board_src,
  include_directories: [inc_src],
)

#- Exportable Module -#
board_dep = declare_dependency(
  link_with: board_lib,
  include_directories: [inc_src],
)