cmake_minimum_required(VERSION 3.28)

project(ABSCtl LANGUAGES CXX VERSION 0.0)

set(MAIN_SRC
  src/main/main.cpp
  src/core/worker.cpp
  src/core/pkg_checker.cpp
  src/core/exceptions/exception_handler.cpp
  src/parsers/argparse.cpp
  src/parsers/configparse.cpp
  src/util/util.cpp
PARENT_SCOPE)
