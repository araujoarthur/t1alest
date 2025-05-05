libdbll_src = files('src/libdbll.c')
libdbll = static_library('gll', libdbll_src,
  include_directories: include_directories('src'),
  install: false
)

libdbll_dep = declare_dependency(
  include_directories: include_directories('src'),
  link_with: libdbll
)
meson.override_dependency('libgll', libdbll_dep)

custom_target('copy-libgll-header',
  input: 'src/libdbll.h',
  output: 'libgll.h',
  command: ['sh', '-c', 'mkdir -p ../../dependencies/include && cp @INPUT@ ../../dependencies/include/libgll.h'],
  build_by_default: true
)

custom_target('copy-libgll-a',
  output: 'copied-libgll.stamp',
  depends: libdbll,
  command: ['sh', '-c', 'mkdir -p ../../dependencies/lib && cp @0@ ../../dependencies/lib/libgll.a'.format(libdbll.full_path())],
  build_by_default: true
)