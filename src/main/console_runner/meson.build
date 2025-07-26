#- Source File -#
console_runner_src = files('console_runner.cpp')

#- Include Dirs -#
incs = [inc_src]

#- Logging Library -#
console_runner_lib = library(
  'console_runner',
  console_runner_src,
  include_directories: incs,
  dependencies: [game_dep, console_ui_dep]
)

#- Exportable Module -#
console_runner_dep = declare_dependency(
  link_with: console_runner_lib,
  include_directories: incs,
  dependencies: [game_dep, console_ui_dep]
)
