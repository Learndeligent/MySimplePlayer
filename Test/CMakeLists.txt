cmake_minimum_required (VERSION 3.8)

include_directories(../)
include_directories(/usr/local/Cellar/ffmpeg/4.2.1_1/include)
link_directories(/usr/local/Cellar/ffmpeg/4.2.1_1/lib)

add_executable(
    test
    test.cpp
)

target_link_libraries(test MyPlayer MyThread)
target_link_libraries(
    test
    avcodec
    avdevice
    avformat
    avutil
    swresample
    swscale
)
