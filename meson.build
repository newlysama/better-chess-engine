project('chess', 'cpp',
  version : '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'wrap_mode=forcefallback',
    'warning_level=3',
    'default_library=static',
    ]
)

#------------------------------#
#        PROJECT MODE          #
#------------------------------#

mode            = get_option('mode')
generate_magics = get_option('generate_magics')
console         = get_option('console')
server          = get_option('server')

# Global project arguments
add_project_arguments(
  '-DPROJECT_ROOT="' + meson.project_source_root() + '"',
  language: 'cpp'
)

# Performance arguments
if mode in ['profile', 'benchmark', 'release']
  add_project_arguments(
    '-Doptimization=3',
    '-march=native',
    '-mtune=native',
    '-funroll-loops',
    '-fomit-frame-pointer',
    language: 'cpp',
  )
endif

if mode == 'debug'
  add_project_arguments(
    '-DBUILD_DEBUG',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE',
    '-Db_sanitize=address',
    '-Db_lundef=false',
    '-Doptimization=0',
    language: 'cpp'
  )

elif mode == 'profile'
  add_project_arguments(
    '-DBUILD_PROFILE',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE',
    '-Db_pch=true',
    language: 'cpp'
  )

elif mode == 'release'
  add_project_arguments(
    '-DBUILD_RELEASE',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO',
    language: 'cpp'
  )

elif mode == 'test'
  add_project_arguments(
    '-DBUILD_TEST',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE',
    '-Db_sanitize=address',
    '-Db_lundef=false',
    '-Doptimization=0',
    language: 'cpp'
  )

  add_project_link_arguments(
    '-pthread',
    language: 'cpp'
  )

elif mode == 'benchmark'
  add_project_arguments(
    '-DBUILD_BENCHMARK',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO',
    '-Db_pch=true',
    language: 'cpp'
  )

  add_project_link_arguments(
    '-pthread',
    language: 'cpp'
  )
endif

if generate_magics == true
  if (server == true) or mode in ['profile', 'benchmark', 'release']
    error('Cannot use generate_magics with server option or release/test/benchmark build modes.')
  else
    add_project_arguments(
      '-DGENERATE_MAGICS',
      language: 'cpp'
    )

  add_project_link_arguments(
    '-fopenmp',
    language: 'cpp'
  )
  endif
endif




#------------------------------#
#             DEPS             #
#------------------------------#

# spdlog (subprojects/spdlog.wrap → export spdlog_dep)
spdlog_dep = dependency(
  'spdlog',
  fallback: ['spdlog', 'spdlog_dep'],
  required: true
)

# GoogleTest (subprojects/gtest.wrap → export gtest_dep)
if mode == 'test'
  gtest_dep = dependency(
    'gtest',
    fallback: ['gtest', 'gtest_dep'],
    required: mode == 'test'
  )
endif

# Google Benchmark (subprojects/google-benchmark.wrap → export benchmark_dep)
if mode == 'benchmark'
  benchmark_dep = dependency(
    'google-benchmark',
    fallback: ['google-benchmark', 'google_benchmark_dep'],
    required: mode == 'benchmark'
  )
endif

#------------------------------#
#         INCLUDE DIRS         #
#------------------------------#

inc_src = include_directories('src')


#------------------------------#
#           SUB DIRS           #
#------------------------------#

subdir('src/logging')
subdir('src/engine/board')
subdir('src/engine/game')

if mode not in ['test', 'benchmark']
  subdir('src/main')
endif