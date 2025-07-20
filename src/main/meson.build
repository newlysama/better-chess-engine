#- Source Files -#
app_src = files('main.cpp')

#- Include Dirs -#
incs = [inc_src]

deps = [logging_dep, game_dep]

if console == true
  deps += console_runner_dep
endif

#- Chess Executable -#
executable(
  'chess',
  app_src,
  include_directories: incs,
  dependencies: deps
)
