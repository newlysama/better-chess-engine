#- Source File -#
logging_src = files('logging.cpp')

#- Include Dirs -#
incs = [inc_src]

#- Logging Library -#
logging_lib = library(
  'logging',
  logging_src,
  include_directories: incs,
  dependencies: [spdlog_dep]
)

#- Exportable Module -#
logging_dep = declare_dependency(
  link_with: logging_lib,
  include_directories: incs,
  dependencies: [spdlog_dep]
)
