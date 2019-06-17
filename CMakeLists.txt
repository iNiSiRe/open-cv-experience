cmake_minimum_required(VERSION 2.8.4)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project( cv )

find_package( OpenCV REQUIRED )
#find_package( FFmpeg REQUIRED )

include_directories("${OpenCV_INCLUDE_DIRS}")
#include_directories("${FFMPEG_INCLUDE_DIRS}")

add_executable( cv src/main.cpp src/tcp/TCPClient.cpp src/tcp/TCPClient.h )

target_link_libraries( cv ${OpenCV_LIBS} crypto ssl)