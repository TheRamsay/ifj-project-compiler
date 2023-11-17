# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ramsay/school/ifj/project/build/googletest-src"
  "/home/ramsay/school/ifj/project/build/googletest-build"
  "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix"
  "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix/tmp"
  "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix/src/googletest-stamp"
  "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix/src"
  "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ramsay/school/ifj/project/build/googletest-download/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
