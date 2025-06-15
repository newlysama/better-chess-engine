#- Source Files -#
engine_src = files(
  'src/bitboard.cpp',
  'src/board.cpp',
  'src/game.cpp'
)

#- Include Dirs -#
incs = [inc_src]

#- Engine Library -#
engine_lib = library(
  'engine',
  engine_src,
  include_directories: incs,
  dependencies: [spdlog_dep]
)

#- Exportable Module -#
engine_dep = declare_dependency(
  link_with: engine_lib,
  include_directories: incs,
  dependencies: [spdlog_dep]
)
