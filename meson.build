project('slay',
  'cpp', version:
  '0.1', default_options: [
  'cpp_std=c++20',
  'warning_level=3',
] )

add_project_arguments(
    '-Wno-missing-field-initializers',
    language: 'cpp'
)

sdl_dep = dependency('sdl3', required: false)

clay_sp = subproject('clay')
clay_dep = clay_sp.get_variable('clay_dep')

slay_inc = include_directories('./include')
slay_files = files('src/app.cpp', 'src/renderer/sdl_renderer.cpp', 'src/layout_engine.cpp', 'src/platform.cpp')

slay_lib = static_library('slay', slay_files, include_directories: slay_inc, dependencies: [sdl_dep, clay_dep])

slay_dep = declare_dependency(
  link_with: slay_lib,
  include_directories: slay_inc,
  dependencies: [
    sdl_dep,
    clay_dep,
  ],
)

subdir('examples/hello_slay')
