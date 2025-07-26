#----- src/main/console_runner/meson.build -----#

#- Source File -#
console_runner_src = files('console_runner.cpp')

#- Console Runner Library -#
console_runner_lib = library(
  'console_runner',
  console_runner_src,
  include_directories: [inc_src],
  dependencies: [logging_dep, engine_dep, console_ui_dep]
)

#- Exportable Module -#
console_runner_dep = declare_dependency(
  link_with: console_runner_lib,
  include_directories: [inc_src],
  dependencies: [logging_dep, engine_dep, console_ui_dep]
)
