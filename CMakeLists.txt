cmake_minimum_required(VERSION 3.8...3.25.2)

project(PossionRecon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Global properties.
set_property(GLOBAL PROPERTY CXX_EXCEPTIONS ON)
set_property(GLOBAL PROPERTY CXX_RTTI ON)

# The required dependencies.
find_package(jpeg REQUIRED)
find_package(PNG REQUIRED)
find_package(zlib REQUIRED)

option(POISSON_APPS "Build the apps." ON)   # Build the command line apps.
option(POISSON_LIB "Build the apps." ON)    # Build the library.

include_directories(${JPEG_INCLUDE_DIR})
include_directories(${PNG_INCLUDE_DIR})

if(POISSON_APPS)
    add_executable(PoissonRecon Src/PoissonRecon.cpp)
    target_link_libraries(PoissonRecon ${JPEG_LIBRARIES})
    target_link_libraries(PoissonRecon ${PNG_LIBRARIES})
    target_link_libraries(PoissonRecon ${ZLIB_LIBRARIES})
    install(TARGETS PoissonRecon EXPORT PoissonRecon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    add_executable(SSDRecon Src/SSDRecon.cpp)
    target_link_libraries(SSDRecon ${JPEG_LIBRARIES})
    target_link_libraries(SSDRecon ${PNG_LIBRARIES})
    target_link_libraries(SSDRecon ${ZLIB_LIBRARIES})
    install(TARGETS SSDRecon EXPORT SSDRecon RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    add_executable(ImageStitching Src/ImageStitching.cpp)
    target_link_libraries(ImageStitching ${JPEG_LIBRARIES})
    target_link_libraries(ImageStitching ${PNG_LIBRARIES})
    target_link_libraries(ImageStitching ${ZLIB_LIBRARIES})
    install(TARGETS ImageStitching EXPORT ImageStitching RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(POISSON_LIB)
    message(INFO "Not implemented yet.")
endif()