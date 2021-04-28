# pragma once

#include "MyReaderThread.h"
#include "MyCtrThread.h"
#include "MyEnum.h"

#include <string>

class MyPlayer
{
public:
    MyPlayer(std::string _path);
    ~MyPlayer();

    int Play();

    MyReaderThread* getReader();
    MyCtrThread* getCtr();

    // build
    int BuildReader();
    int BuildDecoder();
    int BuildController();

    // configue
    int Configue();

    // start
    int StartReader();
    int StartDecoder();
    int StartController();

    // stop
    int StopAll();
private:
    std::string path = "";
    MyReaderThread* reader = nullptr;
    MyDecoderThread* decoder_video = nullptr;
    MyDecoderThread* decoder_audio = nullptr;
    MyCtrThread* controller = nullptr;
};