#pragma once

#include "MyThread/MyThread.h"
#include "MyDecoderThread.h"

extern "C"
{
#include <libavformat/avformat.h>
}

#include <string>

class MyReaderThread : public MyThread
{
public:
    MyReaderThread(std::string _path);
    ~MyReaderThread();

    // Open() should be processed in Constructor
    int Open();

    // Close() should be processed in Deconstructor
    int Close();

    int readPacket(AVPacket* p_pkt);

    virtual void run();

    AVFormatContext* getFormatCtx();

    int SetDecoder_video(MyDecoderThread* _decoder_video);
    int SetDecoder_audio(MyDecoderThread* _decoder_audio);

private:
    AVFormatContext* avFormatCtx = nullptr;
    std::string path = "";
    MyDecoderThread* decoder_video = nullptr;
    MyDecoderThread* decoder_audio = nullptr;
};