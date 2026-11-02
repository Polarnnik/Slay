test_arena = executable(
  'test_arena',
  ['test_arena.cpp', '../src/core/arena.cpp'],
  include_directories: include_directories('../include'),
)
test('arena', test_arena)

test_layout = executable('test_layout', 'test_layout.cpp', dependencies: slay_dep)
test('layout', test_layout)
