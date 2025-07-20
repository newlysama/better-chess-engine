#- Source Files -#
game_src = files(
  'move_list.cpp',
  'game.cpp'
)

#- Include Dirs -#
incs = [inc_src]

#- Game Library -#
game_lib = library(
  'game',
  game_src,
  include_directories: incs,
  dependencies: [logging_dep, board_dep]
)

#- Exportable Module -#
game_dep = declare_dependency(
  link_with: game_lib,
  include_directories: incs,
  dependencies: [logging_dep, board_dep]
)
