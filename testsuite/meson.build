srcs = files('src/test_main.c', 'src/testsuite.c')

exe = executable('testsuite', srcs,
  include_directories: include_directories('src'),
  dependencies: [dependency('libgal'), dependency('libgll')],
  install: false
)

custom_target('move-testsuite-bin',
  input: exe,
  output: 'testsuite.stamp',
  command: ['sh', '-c', 'mkdir -p ../bin && cp @INPUT@ ../bin/testsuite'],
  build_by_default: true
)
