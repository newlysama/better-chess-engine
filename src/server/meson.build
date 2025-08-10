#----- src/server/meson.build -----#


#- Subdirs -#
subdir('user/')
subdir('snapshot')
subdir('room/')

server_src = files(
    'server.cpp'
)

pistache_dep = dependency(
  'libpistache',
  required: true
)

#- Server Library -#
server_lib = library(
  'server',
  include_directories: [inc_src],
  dependencies: [logging_dep, pistache_dep, user_dep, room_dep]
)

#- Exportable Module -#
server_dep = declare_dependency(
  link_with: server_lib,
  include_directories: [inc_src],
  dependencies: [logging_dep, pistache_dep, user_dep, room_dep]
)
