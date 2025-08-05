#----- tests/meson.build -----#

#- Depedency: Google Test -#
gtest_dep = dependency(
  'gtest',
  required: true
)

gtest_main_dep  = dependency(
  'gtest_main',
  required: true
)

#- Subdirs -#
subdir('engine/')

#- Tests executable -#
test_exe = executable('test_engine',
  sources: tests_engine_src,
  dependencies: [tbb_dep, gtest_dep, gtest_main_dep, tests_engine_dep],
  include_directories: inc_src,
)

test('run-tests', test_exe)