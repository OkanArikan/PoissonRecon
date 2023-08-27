# This is the legacy minimum version flatbuffers supported for a while.
cmake_minimum_required(VERSION 3.8...3.25.2)

project(PossionRecon
        VERSION 1.0.0
        LANGUAGES CXX)


option(POISSON_APPS "Build the apps." ON)   # Build the command line apps.
option(POISSON_LIB "Build the apps." ON)    # Build the library.