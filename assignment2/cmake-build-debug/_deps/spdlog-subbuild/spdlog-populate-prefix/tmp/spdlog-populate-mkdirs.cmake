# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-src"
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-build"
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix"
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/tmp"
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp"
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src"
  "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/dell/Yoav/CppCourse/assignment2/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp/${subDir}")
endforeach()
