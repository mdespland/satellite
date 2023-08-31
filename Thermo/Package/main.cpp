#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include <math.h>

#include "mlx90641.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>


// Despite the framerate being ostensibly FPS hz
// The frame is often not ready in time
// This offset is added to the FRAME_TIME_MICROS
// to account for this.
#define OFFSET_MICROS 850

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *texture_r = NULL;
SDL_Event event;

SDL_Rect rect_preserve_aspect;
SDL_Rect rect_fullscreen;

bool running = true;
bool preserve_aspect = true;

int rotation = 0;
MLX90641 * mlx90641;

int main(void) {


    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init() Failed: %s\n", SDL_GetError());
        return 1;
    }
        
    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    window = SDL_CreateWindow("Waveshare",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED ,240, 320, SDL_WINDOW_OPENGL| SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow() Failed: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer() Failed: %s\n", SDL_GetError());
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SENSOR_W, SENSOR_H);
    if(texture == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTexture() Failed: %s\n", SDL_GetError());
    }

    texture_r = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, SENSOR_H, SENSOR_H);
    if(texture_r == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTexture() Failed: %s\n", SDL_GetError());
    }


    TTF_Font *font;
    font=TTF_OpenFont("/home/mde/Thermo/Mlx90641-D55_Thermal_Camera_Code/raspberrypi/cpp/Source/Alibaba-PuHuiTi-Medium.ttf", 20);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    char text_buffer[36];
    SDL_Surface* min_temp_text;
    SDL_Surface* max_temp_text;

    SDL_Color blue_color = { 0, 0, 255 };
    SDL_Color red_color = { 0, 0, 255 };


    int display_width, display_height;

    SDL_GetRendererOutputSize(renderer, &display_width, &display_height);

    int aspect_scale = display_height / SENSOR_H;

    int output_width, output_height;

    output_width = SENSOR_W * aspect_scale;
    output_height = SENSOR_H * aspect_scale;

    int offset_left, offset_top;

    offset_left = (display_width - output_width) / 2;
    offset_top = (display_height - output_height) / 2;

    rect_preserve_aspect = (SDL_Rect){.x = offset_left, .y = offset_top, .w = output_width, .h = output_height};
    rect_fullscreen = (SDL_Rect){.x = 0, .y = 0, .w = display_width, .h = display_height};

    mlx90641=new MLX90641();

/*   static uint16_t eeMLX90641[832];
    float emissivity = 1;
    uint16_t frame[834];
    static float image[192];
    static float mlx90641To[192];
    float eTa;
    static uint16_t data[192*sizeof(float)];

    auto frame_time = std::chrono::microseconds(FRAME_TIME_MICROS + OFFSET_MICROS);*/

    //MLX90641_SetDeviceMode(MLX_I2C_ADDR, 0);
    //MLX90641_SetSubPageRepeat(MLX_I2C_ADDR, 0);

    auto frame_time = std::chrono::microseconds(FRAME_TIME_MICROS + OFFSET_MICROS);

    SDL_Texture* min_temp_text_texture;
    SDL_Texture* max_temp_text_texture;
    while(running){
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym){
                    case SDLK_SPACE:
                        preserve_aspect = !preserve_aspect;
                        break;
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_r:
                        rotation += 90;
                        if(rotation == 360){
                            rotation = 0;
                        }
                        break;
                }
            }
        }

	    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SENSOR_W, SENSOR_H);
	    if(texture == NULL){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTexture() Failed: %s\n", SDL_GetError());
	    }

	    texture_r = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, SENSOR_H, SENSOR_H);
	    if(texture_r == NULL){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTexture() Failed: %s\n", SDL_GetError());
	    }

        
        auto start = std::chrono::system_clock::now();
	SDL_UpdateTexture(texture, NULL, (uint8_t *)mlx90641->getImage(), SENSOR_W * sizeof(uint32_t));

        SDL_SetRenderTarget(renderer, texture_r);
        SDL_RenderCopyEx(renderer, texture, NULL, NULL, rotation, NULL, SDL_FLIP_NONE);
        SDL_SetRenderTarget(renderer, NULL);

        if(preserve_aspect){
            SDL_RenderCopy(renderer, texture_r, NULL, &rect_preserve_aspect);
        }
        else
        {
            SDL_RenderCopy(renderer, texture_r, NULL, &rect_fullscreen);
        }

        sprintf(text_buffer,"Min:%d°C",mlx90641->getMinTemp());
        min_temp_text = TTF_RenderUTF8_Solid( font, text_buffer, blue_color );
        sprintf(text_buffer,"Max:%d°C",mlx90641->getMaxTemp());
        max_temp_text = TTF_RenderUTF8_Solid( font, text_buffer, red_color );
	    
	min_temp_text_texture = SDL_CreateTextureFromSurface( renderer, min_temp_text );
	    SDL_Rect min_temp_dest = { 0 , (320 - min_temp_text->h), min_temp_text->w, min_temp_text->h };
	    
	    max_temp_text_texture = SDL_CreateTextureFromSurface( renderer, max_temp_text );
	    SDL_Rect max_temp_dest = { (240 - max_temp_text->w), (320 - max_temp_text->h), max_temp_text->w, max_temp_text->h };

        SDL_RenderCopy( renderer, min_temp_text_texture, NULL, &min_temp_dest );
        SDL_RenderCopy( renderer, max_temp_text_texture, NULL, &max_temp_dest );
	
        SDL_RenderPresent(renderer);
        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::this_thread::sleep_for(std::chrono::microseconds(frame_time - elapsed));
	SDL_DestroyTexture(min_temp_text_texture);
	SDL_DestroyTexture(max_temp_text_texture);
	SDL_DestroyTexture(texture);
	SDL_RenderClear(renderer);
    }

    //SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
