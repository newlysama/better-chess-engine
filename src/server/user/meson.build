#----- src/server/user/meson.build -----#

#- Source File -#
user_src = files(
  'user.cpp'
)

#- User Library -#
user_lib = library(
  'user',
  user_src,
  include_directories: [inc_src],
  dependencies: []
)

#- Exportable Module -#
user_dep = declare_dependency(
  link_with: user_lib,
  include_directories: [inc_src],
  dependencies: []
)
