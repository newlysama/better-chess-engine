#- Source File -#
console_ui_src = files('console_ui.cpp')

#- Include Dirs -#
incs = [inc_src]

#- Logging Library -#
console_ui_lib = library(
  'console_ui',
  console_ui_src,
  include_directories: incs,
  dependencies: [board_dep]
)

#- Exportable Module -#
console_ui_dep = declare_dependency(
  link_with: console_ui_lib,
  include_directories: incs,
  dependencies: [board_dep]
)
