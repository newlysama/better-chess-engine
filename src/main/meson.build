#----- src/main/meson.build -----#

#- Source File -#
app_src = files('main.cpp')

#- Depedencies -#
deps = [logging_dep, engine_dep]

# If we compile with -Dconsole=true, compile the console_runner
if console == true
  subdir('console_runner/')
  deps += console_runner_dep
endif

#- Chess Executable -#
executable(
  'chess',
  app_src,
  include_directories: [inc_src],
  dependencies: deps
)
