cmake_minimum_required(VERSION 3.10)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb")

# set the project name and version
project(FEC VERSION 1.0)

# specify C standard
set(CMAKE_CXX_STANDARD 11)

# configure a header file to pass some of the CMake settings to the source code
configure_file(inc/const.h.in const.h)

# add the executable
# m link math library
aux_source_directory(./src CXX_SRC)
add_executable(FEC ${CXX_SRC})
target_link_libraries(FEC PUBLIC 
    ${EXTRA_LIBS}
    m
    )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(FEC PUBLIC
                        "${PROJECT_BINARY_DIR}"
                        )