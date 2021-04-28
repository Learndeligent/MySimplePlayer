#include "MyReaderThread.h"

#include <iostream>

MyReaderThread::MyReaderThread(std::string _path)
{
    path = _path;
    Open();
}

MyReaderThread::~MyReaderThread()
{
    Close();
}

int MyReaderThread::Open()
{
    avFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&avFormatCtx, path.c_str(), nullptr, nullptr);
    if(ret) {
        std::cout << "inputFile open failed!" << std::endl;
        return -1;
    } else {
        avformat_find_stream_info(avFormatCtx, nullptr);
        std::cout << "inputFile open Success!" << std::endl;
        return 0;
    }
}

int MyReaderThread::Close()
{
    if (avFormatCtx != nullptr) {
        avformat_close_input(&avFormatCtx);
    }
    if (avFormatCtx != nullptr) {
        avformat_free_context(avFormatCtx);
        avFormatCtx = nullptr;
    }
    return 0;
}

int MyReaderThread::readPacket(AVPacket* p_pkt)
{
    int ret = av_read_frame(avFormatCtx, p_pkt);
    return ret;
}

void MyReaderThread::run()
{
    int videoStreamIndex = av_find_best_stream(avFormatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);
	int audioStreamIndex = av_find_best_stream(avFormatCtx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, NULL, NULL);

    int c_vp = 0, c_ap = 0;
    while(!stopFlag) {
        // if too many packets can't be processed, deny this thread
        if(decoder_video -> SizePacketQueue() > 5 || decoder_audio -> SizePacketQueue() > 5) {
            continue;
        }

        AVPacket* p_pkt = new AVPacket();
        int ret = readPacket(p_pkt);
        if (ret) {
            delete p_pkt;
            p_pkt = nullptr;
            // if no packet to read, should end the thread
            break;
        }
        int streamIndex = p_pkt->stream_index;
        if (streamIndex == videoStreamIndex) {
            // if video packet
            decoder_video->pushPacket(p_pkt);
            std::cout << "Video packet Put #" << ++c_vp << std::endl;
        } else if(streamIndex == audioStreamIndex) {
            // if audio packet
            decoder_audio->pushPacket(p_pkt);
            std::cout << "Audio packet Put #" << ++c_ap << std::endl;
        }
     
    }
    
}

AVFormatContext* MyReaderThread::getFormatCtx()
{
    return avFormatCtx;
}

int MyReaderThread::SetDecoder_video(MyDecoderThread* _decoder_video)
{
    decoder_video = _decoder_video;
    return 0;
}

int MyReaderThread::SetDecoder_audio(MyDecoderThread* _decoder_audio)
{
    decoder_audio = _decoder_audio;
    return 0;
}
