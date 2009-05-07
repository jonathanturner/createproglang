project(sifula)

SET (CMAKE_BUILD_TYPE gdb)
SET (CMAKE_C_FLAGS_GDB " -Wall -ggdb")
SET (CMAKE_CXX_FLAGS_GDB " -Wall -ggdb")

add_executable(sifula lexer.cpp parser.cpp interpreter.cpp main.cpp eval.cpp)
