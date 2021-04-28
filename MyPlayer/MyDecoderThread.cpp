#include "MyDecoderThread.h"
#include <iostream>
#include <chrono>

MyDecoderThread::MyDecoderThread(AVFormatContext* _avFormatCtx, CodecType _codectype)
{
    avFormatCtx = _avFormatCtx;
    codectype = _codectype;
    codecCtx = avcodec_alloc_context3(nullptr);

    init();
}

MyDecoderThread::~MyDecoderThread()
{
    // free PacketQueue
    AVPacket* p_pkt;
    while(PacketQueue.size()) {
        PacketQueue.pop(&p_pkt);
        delete p_pkt;
        p_pkt = nullptr;
    }
    AVFrame* p_frame;
    while(FrameQueue.size()) {
        FrameQueue.pop(&p_frame);
        delete p_frame;
        p_frame = nullptr;
    }
    
    // free FrameQueue
}

void MyDecoderThread::run()
{
    std::cout << "this is MyDecoderThread!" << std::endl;
    int c_frame = 0;
#if YUVWRITE
    FILE* f = fopen("/Users/feihong/Desktop/demo_video.yuv", "wb");
#endif
    while(!stopFlag) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (SizeFrameQueue() > 20) {
            continue;
        }

        // step 1 : take a packet from the queue
        AVPacket* p_pkt = nullptr;
        int ret = PacketQueue.pop(&p_pkt);
        if (ret){ 
            continue;
        }
        // step 2 : send packet
        ret = sendPacket(p_pkt);
        if(ret) {
            continue;
        }

        // step 3 : receive frame
        while (1) {
            AVFrame* p_frame = new AVFrame();
            ret = RecvFrame(p_frame);
            if (ret) {
                // no frame to receive
                break;
            }

            // receive frame success
            if (codectype == CodecType::VIDEO) {
                FrameQueue.push(p_frame);
                // std::cout << "VideoFrame#" << ++c_frame << " Success!" << std::endl;
            } else if (codectype == CodecType::AUDIO) {
                //std::cout << "AudioFrame#" << ++c_frame << " Success!" << std::endl;
            }

#if YUVWRITE
            // wirte the yuv data
            if (codectype == CodecType::VIDEO) {
                int width = p_frame->width, height = p_frame->height;
                int half_width = width/2, half_height = height/2;
                unsigned char* y = (unsigned char*)malloc(width * height);
                unsigned char* u = (unsigned char*)malloc(width / 2 * height / 2);
                unsigned char* v = (unsigned char*)malloc(width / 2 * height / 2);

                // y
                for (int i = 0; i < height;i++) {
                    memcpy(y + i * width, p_frame->data[0] + i * p_frame->linesize[0], width);
                }

                // u
                for (int i = 0; i < half_height;i++) {
                    memcpy(u + i * half_width, p_frame->data[1] + i * p_frame->linesize[1], half_width);
                }

                // v
                for (int i = 0; i < half_height;i++) {
                    memcpy(v + i * half_width, p_frame->data[2] + i * p_frame->linesize[2], half_width);
                }

                fwrite(y, width*height, 1, f);
                fwrite(u, half_width*half_height, 1, f);
                fwrite(v, half_width*half_height, 1, f);

                free(y);
                free(u);
                free(v);
            }
#endif
        }
        delete p_pkt;
        p_pkt = nullptr;
    }
    
#if YUVWRITE
    fclose(f);
#endif

}

int MyDecoderThread::sendPacket(AVPacket * p_pkt)
{
    int ret = avcodec_send_packet(codecCtx, p_pkt);

    return ret;
}

int MyDecoderThread::RecvFrame(AVFrame * p_frame)
{
    int ret = avcodec_receive_frame(codecCtx, p_frame);

    return ret;
}

void MyDecoderThread::init()
{
    int StreamIndex;
    if (codectype == CodecType::VIDEO) {
        StreamIndex = av_find_best_stream(avFormatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, NULL, NULL);
    } else {
        StreamIndex = av_find_best_stream(avFormatCtx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, NULL, NULL);
    }

    AVStream* iStream = avFormatCtx->streams[StreamIndex];

    timebaseNum = iStream->time_base.num;
    timebaseDen = iStream->time_base.den;

    avcodec_parameters_to_context(codecCtx, iStream->codecpar);
    codec = avcodec_find_decoder(codecCtx->codec_id);
    
    int ret = avcodec_open2(codecCtx, codec, nullptr);
    if (ret) {
        printf("avcodec_open2 fail ret:%d\n", ret);
    } else {
        if (codectype == CodecType::VIDEO) {
            std::cout << "Video Codec initialized Success!" << std::endl;
        } else if (codectype == CodecType::AUDIO){
            std::cout << "Audio Codec initialized Success!" << std::endl;
        }
    }

}

int MyDecoderThread::pushPacket(AVPacket * p_pkt)
{
    PacketQueue.push(p_pkt);
    return 0;
}

int MyDecoderThread::popPacket(AVPacket ** pp_pkt)
{
    return PacketQueue.pop(pp_pkt);
}

int MyDecoderThread::SizePacketQueue()
{
    return PacketQueue.size();
}

int MyDecoderThread::pushFrame(AVFrame * p_frame)
{
    FrameQueue.push(p_frame);
    return 0;
}

int MyDecoderThread::popFrame(AVFrame ** p_frame)
{
    return FrameQueue.pop(p_frame);
}

int MyDecoderThread::SizeFrameQueue()
{
    return FrameQueue.size();
}

AVCodecContext* MyDecoderThread::getCodecCtx()
{
    return codecCtx;
}