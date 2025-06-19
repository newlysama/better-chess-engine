#- Source Files -#
board_src = files('board.cpp')

#- Include Dirs -#
incs = [inc_src]

#- Board Library -#
board_lib = library(
  'board',
  board_src,
  include_directories: incs
)

#- Exportable Module -#
board_dep = declare_dependency(
  link_with: board_lib,
  include_directories: incs
)
