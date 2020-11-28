cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(OFDM VERSION 1.0)

# specify C standard
set(CMAKE_C_STANDARD 99)

# configure a header file to pass some of the CMake settings to the source code
configure_file(inc/const.h.in const.h)

# add the executable
add_executable(OFDM src/main.c src/ber.c src/channel.c src/receiver.c src/transmitter.c)
target_link_libraries(OFDM PUBLIC 
    ${EXTRA_LIBS}
    m
    )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(OFDM PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )