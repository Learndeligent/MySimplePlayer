#include "MyCtrThread.h"

#include <iostream>
#include <thread>

MyCtrThread::MyCtrThread()
{

}


MyCtrThread::~MyCtrThread()
{

}

void MyCtrThread::run()
{

}

int thread_exit=0;
int refresh_video(void *opaque)
{
    while (thread_exit==0) {
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(40);
	}
	return 0;
}

int MyCtrThread::myrun()
{
    std::cout << "this is MyCtrThread!" << std::endl;

    int c_frame = 0;

    // sdl
    // step 0
    if(SDL_Init(SDL_INIT_VIDEO)) {  
		std::cout << SDL_GetError() << std::endl;
		return -1;
	} 
    // step 1
    SDL_Window *screen; 
    int screen_w = 1000, screen_h = 500;
	//SDL 2.0 Support for multiple windows
	screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if(!screen) {  
		std::cout << SDL_GetError() << std::endl;
		return -1;
	}

    // step 2
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  
 
	Uint32 pixformat= SDL_PIXELFORMAT_IYUV;  
    int pixel_w = decoder_video->getCodecCtx()->width;
    int pixel_h = decoder_video->getCodecCtx()->height;
    
    unsigned char buffer[pixel_w*pixel_h*12/8];

    SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer,pixformat, SDL_TEXTUREACCESS_STREAMING,pixel_w,pixel_h);

    SDL_Rect sdlRect; 
    SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video,NULL,NULL);
    SDL_Event event;

    while(!stopFlag) {
        AVFrame* p_frame = nullptr;
        int ret = decoder_video->popFrame(&p_frame);
        if (ret) {
            // no frame in FrameQueue
            std::cout << "No Frame to Read!" << std::endl;
        } else {
            // 
        }
        
        SDL_WaitEvent(&event);
		if(event.type==REFRESH_EVENT){
            if (p_frame != nullptr) {
                int width = p_frame->width, height = p_frame->height;
                int half_width = width/2, half_height = height/2;

                // TODO: display
                std::cout << "Ctr Video Frame# " << ++c_frame << std::endl;
                // 获取buffer
                // y
                for (int i = 0; i < height;i++) {
                    memcpy(buffer + i * width, p_frame->data[0] + i * p_frame->linesize[0], width);
                }
                // u
                for (int i = 0; i < half_height;i++) {
                    memcpy(buffer + height * width + i * half_width, p_frame->data[1] + i * p_frame->linesize[1], half_width);
                }
                // v
                for (int i = 0; i < half_height;i++) {
                    memcpy(buffer + height * width * 5 / 4 + i * half_width, p_frame->data[2] + i * p_frame->linesize[2], half_width);
                }

                // step2-1 : 设置纹理数据
                SDL_UpdateTexture( sdlTexture, NULL, buffer, pixel_w);  
                //FIX: If window is resize
                sdlRect.x = 0;  
                sdlRect.y = 0;  
                sdlRect.w = screen_w;  
                sdlRect.h = screen_h;  
                
                //step2-2 : 将纹理数据拷贝至渲染器中
                SDL_RenderClear( sdlRenderer );   
                SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, &sdlRect);  

                //step2-3 : 在窗口上显示渲染器
                SDL_RenderPresent( sdlRenderer );  
                //Delay 30ms
                SDL_Delay(30);
            }
		}else if(event.type==SDL_WINDOWEVENT){
			//If Resize
			SDL_GetWindowSize(screen,&screen_w,&screen_h);
		}else if(event.type==SDL_QUIT){
            SDL_DestroyWindow(screen);
			break;
		}


        delete p_frame;
        p_frame = nullptr;
    }

    return 0;
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

