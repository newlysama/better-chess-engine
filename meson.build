project('chess', 'cpp',
  version        : '0.1.0',
  default_options: ['cpp_std=c++20', 'wrap_mode=forcefallback']
)

#------------------------------#
#        PROJECT MODE          #
#------------------------------#

mode = get_option('mode')

if mode == 'debug'
  add_project_arguments(
    '-DBUILD_DEBUG',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG',
    language: 'cpp'
  )

elif mode == 'profile'
  add_project_arguments(
    '-DBUILD_PROFILE',
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG',
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
    '-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG',
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
    language: 'cpp'
  )

  add_project_link_arguments(
    '-pthread',
    language: 'cpp'
  )
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

#subdir('src/engine')
subdir('src/logging')

if mode not in ['test', 'benchmark']
  subdir('src/app')
endif

if mode == 'test'
  subdir('tests/engine_tests')
endif

if mode == 'benchmark'
  subdir('benchmarks/engine_benchmarks')
endif
