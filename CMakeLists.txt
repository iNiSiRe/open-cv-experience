cmake_minimum_required(VERSION 2.8.4)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_LEGACY_CYGWIN_WIN32 0)
#set(CMAKE_BUILD_TYPE Release)

project( cv )

find_package( OpenCV REQUIRED )
#find_package( FFmpeg REQUIRED )

include_directories(cv "${OpenCV_INCLUDE_DIRS}")
#include_directories("${FFMPEG_INCLUDE_DIRS}")

add_executable( cv src/main.cpp
#        src/tcp/TCPClient.cpp src/tcp/TCPClient.h
        src/detector/ObjectDetector.cpp src/detector/ObjectDetector.h
        src/detector/Object.cpp src/detector/Object.h
        src/detector/DnnObjectDetector.cpp src/detector/DnnObjectDetector.h
        src/detector/factory/DetectorFactory.cpp src/detector/factory/DetectorFactory.h
        src/detector/ChangeDetector.cpp src/detector/ChangeDetector.h
        src/thread/ObjectDetectorThread.cpp src/thread/ObjectDetectorThread.h
        src/detector/YoloObjectDetector.cpp src/detector/YoloObjectDetector.h)

#set(BUILD_SHARED_LIBRARIES OFF)

#set_target_properties( cv PROPERTIES LINK_FLAGS "-static" )
#set_target_properties( cv PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++" )
#target_link_libraries( cv PUBLIC libc.a libgcc.a libstdc++.a libm.a )

target_link_libraries( cv PUBLIC ${OpenCV_LIBS} )

#target_link_libraries( cv PUBLIC -Wl,-Bstatic stdc++ -Wl,-Bdynamic )
#target_link_libraries( cv PUBLIC -Wl,-Bstatic gcc -Wl,-Bdynamic)
#target_link_libraries( cv PUBLIC -Wl,-Bstatic c -Wl,-Bdynamic)
#target_link_libraries( cv PUBLIC -Wl,-Bstatic m -Wl,-Bdynamic)

target_link_libraries( cv PUBLIC pthread )
