#----- src/engine/game/meson.build -----#

#- Source File -#
game_src = files(
  'move_list.cpp',
  'game.cpp'
)

#- Game Library -#
game_lib = library(
  'game',
  game_src,
  include_directories: [inc_src],
  dependencies: [board_dep]
)

#- Exportable Module -#
game_dep = declare_dependency(
  link_with: game_lib,
  include_directories: [inc_src],
  dependencies: [board_dep]
)
