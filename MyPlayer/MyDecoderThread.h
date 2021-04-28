#pragma once
#include "MyEnum.h"
#include "MyThread/MyThread.h"
#include "MyQueue/MyQueue.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

#define YUVWRITE 0

class MyDecoderThread : public MyThread
{
public:
    MyDecoderThread(AVFormatContext* _avFormatCtx, CodecType _codectype);
    ~MyDecoderThread();

    virtual void run();

    void init();

    int pushPacket(AVPacket * p_pkt);

    int popPacket(AVPacket ** pp_pkt);

    int sendPacket(AVPacket * p_pkt);

    int RecvFrame(AVFrame * p_frame);

    int pushFrame(AVFrame * p_frame);

    int popFrame(AVFrame ** p_frame);

    int SizePacketQueue();

    int SizeFrameQueue();

    int timebaseNum = 0;
    int timebaseDen = 0;

    AVCodecContext* getCodecCtx();

private:
    AVFormatContext* avFormatCtx = nullptr;
    CodecType codectype;
    AVCodecContext* codecCtx;
    AVCodec* codec;
    MyQueue<AVPacket> PacketQueue;
    MyQueue<AVFrame> FrameQueue;
};