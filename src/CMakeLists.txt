cmake_minimum_required(VERSION 3.28)

project(ABSCtl LANGUAGES CXX VERSION 0.0)

set(MAIN_SRC
  src/main.cpp
  src/argparse.cpp
  src/util.cpp
  src/worker.cpp
  src/pkg_checker.cpp
PARENT_SCOPE)