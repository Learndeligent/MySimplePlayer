#include "MyCtrThread.h"

#include <iostream>

MyCtrThread::MyCtrThread()
{

}


MyCtrThread::~MyCtrThread()
{

}

void MyCtrThread::run()
{
    std::cout << "this is MyCtrThread!" << std::endl;

    int c_frame = 0;

    while(!stopFlag) {
        AVFrame* p_frame = nullptr;
        int ret = decoder_video->popFrame(&p_frame);
        if (ret) {
            // no frame in FrameQueue
            continue;
        }

        // TODO: display
        std::cout << "Ctr Video Frame# " << ++c_frame << std::endl;

        delete p_frame;
        p_frame = nullptr;
    }
}

int MyCtrThread::init()
{

}

int MyCtrThread::SetDecoder_video(MyDecoderThread* _decoder_video)
{
    decoder_video = _decoder_video;
    return 0;
}

int MyCtrThread::SetDecoder_audio(MyDecoderThread* _decoder_audio)
{
    decoder_audio = _decoder_audio;
    return 0;
}