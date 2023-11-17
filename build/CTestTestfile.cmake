# CMake generated Testfile for 
# Source directory: /home/ramsay/school/ifj/project
# Build directory: /home/ramsay/school/ifj/project/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(scanner "Test_scanner")
set_tests_properties(scanner PROPERTIES  _BACKTRACE_TRIPLES "/home/ramsay/school/ifj/project/CMakeLists.txt;48;add_test;/home/ramsay/school/ifj/project/CMakeLists.txt;0;")
add_test(symtable "Test_symtable")
set_tests_properties(symtable PROPERTIES  _BACKTRACE_TRIPLES "/home/ramsay/school/ifj/project/CMakeLists.txt;57;add_test;/home/ramsay/school/ifj/project/CMakeLists.txt;0;")
add_test(stack "Test_stack")
set_tests_properties(stack PROPERTIES  _BACKTRACE_TRIPLES "/home/ramsay/school/ifj/project/CMakeLists.txt;66;add_test;/home/ramsay/school/ifj/project/CMakeLists.txt;0;")
add_test(str "Test_str")
set_tests_properties(str PROPERTIES  _BACKTRACE_TRIPLES "/home/ramsay/school/ifj/project/CMakeLists.txt;75;add_test;/home/ramsay/school/ifj/project/CMakeLists.txt;0;")
add_test(parser "Test_scanner")
set_tests_properties(parser PROPERTIES  _BACKTRACE_TRIPLES "/home/ramsay/school/ifj/project/CMakeLists.txt;85;add_test;/home/ramsay/school/ifj/project/CMakeLists.txt;0;")
subdirs("googletest-build")
