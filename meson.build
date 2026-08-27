project('slay',
  'cpp', version:
  '0.1', default_options: [
  'cpp_std=c++20',
  'warning_level=3',
] )

sdl_dep = dependency('sdl3', required: false)

clay_sp = subproject('clay')
clay_dep = clay_sp.get_variable('clay_dep')

slay_inc = include_directories('./include')
slay_files = files('src/app.cpp')

slay_lib = static_library('slay', slay_files, include_directories: slay_inc, dependencies: [sdl_dep, clay_dep])
