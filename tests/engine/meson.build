#----- tests/engine/meson.build -----#

#- Source File -#
tests_engine_src = files(
    #'test_fen.cpp',
    #'test_game.cpp',
    'test_perft.cpp'
)

#- Tests Library -#
tests_engine_lib = library(
  'tests_engine',
  tests_engine_src,
  include_directories: [inc_src],
  dependencies: [engine_dep]
)

#- Exportable Module -#
tests_engine_dep = declare_dependency(
  link_with: tests_engine_lib,
  include_directories: [inc_src],
  dependencies: [engine_dep]
)
