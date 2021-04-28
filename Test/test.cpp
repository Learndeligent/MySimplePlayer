#include "MyPlayer/MyPlayer.h"
#include "MyPlayer/MyReaderThread.h"
#include "MyPlayer/MyDecoderThread.h"

#include <chrono>

int main()
{
    std::string input_file = "/Users/feihong/Desktop/firework.mp4";

    // step 0 : create a player
    MyPlayer* myplayer = new MyPlayer(input_file);

    // step 1 : create a reader
    myplayer->BuildReader();

    // step 2 : create a decoder
    myplayer->BuildDecoder();

    // step 3 : create a controller
    myplayer->BuildController();

    myplayer->Configue();

    // step 4 : start the reader
    myplayer->StartReader();

    // step 5 : start the decoder
    myplayer->StartDecoder();

    // step 6 : start the controller
    myplayer->StartController();

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    myplayer->StopAll();


    delete myplayer;
    myplayer = nullptr;

    return 0;
}