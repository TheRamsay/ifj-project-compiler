cmake_minimum_required(VERSION 3.12)
project(ifj_project CXX C)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# ------- GoogleTest -------
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# ----------------------------------------------------------------
# ------- App -------
include_directories(src)
add_executable(main
        main.c
        generator.h generator.c
        builtin.h builtin.c
        parser.h parser.c
        error.h error.c
        scanner.h scanner.c
        stack.h stack.c
        str.h str.c
        symtable.h symtable.c)

# ----------------------------------------------------------------
# ------- Tests -------

# add_executable(Test_scanner
#         scanner.h scanner.c
#         tests/scanner.cpp)
# target_link_libraries(Test_scanner gtest gtest_main)

# add_test(scanner Test_scanner)


# ----------------------------------------------------------------
# ------- Symtable -------
add_executable(Test_symtable
        symtable.h symtable.c
        error.h error.c
        builtin.h builtin.c
        tests/symtable.cpp)
target_link_libraries(Test_symtable gtest gtest_main)
add_test(symtable Test_symtable)
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# ------- Stack -------
add_executable(Test_stack
        stack.h stack.c
        tests/stack.cpp)
target_link_libraries(Test_stack gtest gtest_main)
add_test(stack Test_stack)
# ----------------------------------------------------------------

add_executable(Test_str
        str.h str.c     
        tests/str.cpp)
target_link_libraries(Test_str gtest gtest_main)
add_test(str Test_str)

# ----------------------------------------------------------------
# ------- Parser -------
add_definitions(-DPARSER_TEST)
add_executable(Test_parser
        parser.h parser.c
        builtin.h builtin.c
        symtable.h symtable.c
        error.h error.c
        generator.c generator.h
        str.h str.c
        stack.h stack.c
        scanner.c scanner.h
        tests/parser.cpp)
target_link_libraries(Test_parser gtest gtest_main)
add_test(parser Test_parser)

# ----------------------------------------------------------------

# ----------------------------------------------------------------
# ------- Generator -------
add_executable(Test_generator
        generator.h generator.c
        str.h str.c
        error.h error.c
        stack.h stack.c
        symtable.h symtable.c
        tests/generator.cpp)
target_link_libraries(Test_generator gtest gtest_main)

add_test(generator Test_generator)

add_definitions(-DPARSER_TEST)
add_executable(ExpParserTest
        exp_parser.h exp_parser.c
        parser.h parser.c
        builtin.h builtin.c
        symtable.h symtable.c
        error.h error.c
        generator.c generator.h
        str.h str.c
        stack.h stack.c
        scanner.c scanner.h
        tests/exp_parser.cpp)
target_link_libraries(ExpParserTest gtest gtest_main)
