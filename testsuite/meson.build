project('testsuite', 'c', default_options: ['c_std=gnu2x'])

src = files('src/testsuite.c', 'src/test_main.c')

libgal_dep = declare_dependency(
   include_directories: include_directories('../../include'),
   link_args: ['../../lib/libgal.a', '../../lib/libgll.a']
)

executable('tsuite', src, dependencies: libgal_dep)