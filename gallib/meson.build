gallib_src = files('src/gal.c')
gallib = static_library('gal', gallib_src,
  include_directories: include_directories('src'),
  install: false
)

gallib_dep = declare_dependency(
  include_directories: include_directories('src'),
  link_with: gallib
)
meson.override_dependency('libgal', gallib_dep)

custom_target('copy-gal-header',
  input: 'src/gal.h',
  output: 'libgal.h',
  command: ['sh', '-c', 'mkdir -p ../../dependencies/include && cp @INPUT@ ../../dependencies/include/libgal.h'],
  build_by_default: true
)

custom_target('copy-libgal-a',
  output: 'copied-libgal.stamp',  # dummy output file
  depends: gallib,
  command: [
    'sh', '-c',
    'mkdir -p ../../dependencies/lib && cp @0@ ../../dependencies/lib/libgal.a && touch @1@'.format(gallib.full_path(), '@OUTPUT0@')
  ],
  build_by_default: true
)
