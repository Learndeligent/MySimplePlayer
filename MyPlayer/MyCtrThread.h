#pragma once

#include "MyThread/MyThread.h"
#include "MyDecoderThread.h"

#include <chrono>

// ffmpeg
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

// sdl2
extern "C"
{
    #include <SDL2/SDL.h>
}

#define LOAD_BGRA    0
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_YUV420P 1
 
//Bit per Pixel
#if LOAD_BGRA
const int bpp=32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp=24;
#elif LOAD_YUV420P
const int bpp=12;
#endif

#define REFRESH_EVENT  (SDL_USEREVENT + 1)

int refresh_video(void *opaque);
class MyCtrThread : public MyThread
{
public:
    MyCtrThread();
    ~MyCtrThread();

    int init();

    // Don't use it, because sdl_presentation must be run in main_thread
    virtual void run();

    // instread use myrun();
    int myrun();
    
    // 
    int GoForward(int seconds);
    int GoBack(int seconds);

    int Seek(int n_frame, int timepoint);

    int SetDecoder_video(MyDecoderThread* _decoder_video);
    int SetDecoder_audio(MyDecoderThread* _decoder_audio);  

    // sdl
    // static int refresh_video(void *opaque);

private:
    MyDecoderThread* decoder_video = nullptr;
    MyDecoderThread* decoder_audio = nullptr;
};