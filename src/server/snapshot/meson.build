#----- src/server/snapshot/meson.build -----#

#- Source File -#
snapshot_src = files(
  'snapshot_handler.cpp'
)

#- Room Library -#
snapshot_lib = library(
  'snapshot',
  snapshot_src,
  include_directories: [inc_src],
)

#- Exportable Module -#
snapshot_dep = declare_dependency(
  link_with: snapshot_lib,
  include_directories: [inc_src],
)
