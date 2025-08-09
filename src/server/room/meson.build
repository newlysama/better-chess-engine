#----- src/server/room/meson.build -----#

#- Source File -#
room_src = files(
  'room.cpp'
)

#- Room Library -#
room_lib = library(
  'room',
  room_src,
  include_directories: [inc_src],
  dependencies: [logging_dep, engine_dep, user_dep]
)

#- Exportable Module -#
room_dep = declare_dependency(
  link_with: room_lib,
  include_directories: [inc_src],
  dependencies: [logging_dep, engine_dep, user_dep]
)
