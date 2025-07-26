#----- src/logging/meson.build -----#

#- Source File -#
logging_src = files('logging.cpp')

#- Logging framework dep -#
spdlog_dep = dependency(
  'spdlog',
  required: true
)

#- Logging Library -#
logging_lib = library(
  'logging',
  logging_src,
  include_directories: [inc_src],
  dependencies: [spdlog_dep]
)

#- Exportable Module -#
logging_dep = declare_dependency(
  link_with: logging_lib,
  include_directories: [inc_src],
  dependencies: [spdlog_dep]
)
