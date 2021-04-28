cmake_minimum_required (VERSION 3.8)

include_directories(../)
include_directories(/usr/local/Cellar/ffmpeg/4.2.1_1/include)
link_directories(/usr/local/Cellar/ffmpeg/4.2.1_1/lib)

add_library(
    MyPlayer
    MyPlayer.cpp
    MyPlayer.h  
    MyReaderThread.cpp
    MyReaderThread.h 
    MyDecoderThread.cpp
    MyDecoderThread.h 
    MyCtrThread.cpp
    MyCtrThread.h
)

target_link_libraries(MyPlayer MyThread)