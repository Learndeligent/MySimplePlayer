#include "MyPlayer.h"

#include <iostream>

MyPlayer::MyPlayer(std::string _path)
{
    path = _path;
}


MyPlayer::~MyPlayer()
{
    delete reader;
    reader = nullptr;
    delete decoder_video;
    decoder_video = nullptr;
    delete decoder_audio;
    decoder_audio = nullptr;
    delete controller;
    controller = nullptr;
}

int MyPlayer::Play()
{
    return 0;
}

MyReaderThread* MyPlayer::getReader()
{
    return reader;
}

MyCtrThread* MyPlayer::getCtr()
{
    return controller;
}

int MyPlayer::BuildReader()
{
    reader = new MyReaderThread(path);
    return 0;
}

int MyPlayer::BuildDecoder()
{
    decoder_video = new MyDecoderThread(reader->getFormatCtx(), CodecType::VIDEO);
    decoder_audio = new MyDecoderThread(reader->getFormatCtx(), CodecType::AUDIO);    
    return 0;
}

int MyPlayer::BuildController()
{
    controller = new MyCtrThread();
    return 0;
}

int MyPlayer::Configue()
{
    reader->SetDecoder_video(decoder_video);
    reader->SetDecoder_audio(decoder_audio);

    controller->SetDecoder_video(decoder_video);
    controller->SetDecoder_audio(decoder_audio);
}

int MyPlayer::StartReader()
{
    reader->Start();
    return 0;
}

int MyPlayer::StartDecoder()
{
    decoder_video->Start();
    decoder_audio->Start();
    return 0;
}

int MyPlayer::StartController()
{
    controller->myrun();
    return 0;
}

int MyPlayer::StopAll()
{
    // set the stopFlag = 1
    reader->Stop();
    decoder_video->Stop();
    decoder_audio->Stop();
    controller->Stop();
    return 0;
}