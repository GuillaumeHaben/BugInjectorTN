set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(satese
  src/main.cpp
  src/matchers/critical-functions.cpp
  src/matchers/matcher.cpp
  src/matchers/cast-pointer.cpp
  src/matchers/double-free.cpp
  src/matchers/buffer-underflow.cpp
  src/matchers/integer-overflow.cpp
  src/matchers/input.cpp
  src/matchers/inputprof2.cpp
  src/matchers/buffer-write-functions.cpp
  src/matchers/buffer-write-array.cpp
  src/matchers/return-expr.cpp
  src/matchers/while-loop-null-pointer-condition.cpp)

include_directories(src/includes)

target_link_libraries(satese PRIVATE
  clangTooling
  clangBasic
  clangASTMatchers)
