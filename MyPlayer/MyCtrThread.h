#pragma once

#include "MyThread/MyThread.h"
#include "MyDecoderThread.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class MyCtrThread : public MyThread
{
public:
    MyCtrThread();
    ~MyCtrThread();

    int init();
    virtual void run();

    int SetDecoder_video(MyDecoderThread* _decoder_video);
    int SetDecoder_audio(MyDecoderThread* _decoder_audio);  

private:
    MyDecoderThread* decoder_video = nullptr;
    MyDecoderThread* decoder_audio = nullptr;
};