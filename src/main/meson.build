#- Source Files -#
app_src = files('main.cpp')

#- Include Dirs -#
incs = [inc_src]

#- Chess Executable -#
executable(
  'chess',
  app_src,
  include_directories: incs,
  dependencies: [board_dep, logging_dep]
)
