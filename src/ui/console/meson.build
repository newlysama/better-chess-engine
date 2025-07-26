#----- src/ui/console/meson.build -----#

#- Source File -#
console_ui_src = files('console_ui.cpp')

#- Console UI Library -#
console_ui_lib = library(
  'console_ui',
  console_ui_src,
  include_directories: [inc_src],
  dependencies: [board_dep]
)

#- Exportable Module -#
console_ui_dep = declare_dependency(
  link_with: console_ui_lib,
  include_directories: [inc_src],
  dependencies: [board_dep]
)
