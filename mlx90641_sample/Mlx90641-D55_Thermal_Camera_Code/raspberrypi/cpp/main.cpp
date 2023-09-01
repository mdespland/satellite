#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include <math.h>
#include "MLX90641_API.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>

#define MLX_I2C_ADDR 0x33

#define SENSOR_W 12
#define SENSOR_H 16

// Valid frame rates are 1, 2, 4, 8, 16, 32 and 64
// The i2c baudrate is set to 1mhz to support these
#define FPS  4 
#define FRAME_TIME_MICROS (1000000/FPS)

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

uint32_t pixels[SENSOR_W * SENSOR_H];

bool running = true;
bool preserve_aspect = true;

int rotation = 0;

void put_pixel_false_colour(int x, int y, double v) {
    // Heatmap code borrowed from: http://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
    const int NUM_COLORS = 7;
    static float color[NUM_COLORS][3] = { {0,0,0}, {0,0,1}, {0,1,0}, {1,1,0}, {1,0,0}, {1,0,1}, {1,1,1} };
    int idx1, idx2;
    float fractBetween = 0;
    float vmin = 5.0;
    float vmax = 50.0;
    float vrange = vmax-vmin;
    v -= vmin;
    v /= vrange;
    if(v <= 0) {idx1=idx2=0;}
    else if(v >= 1) {idx1=idx2=NUM_COLORS-1;}
    else
    {
        v *= (NUM_COLORS-1);
        idx1 = floor(v);
        idx2 = idx1+1;
        fractBetween = v - float(idx1);
    }

    int ir, ig, ib;

    ir = (int)((((color[idx2][0] - color[idx1][0]) * fractBetween) + color[idx1][0]) * 255.0);
    ig = (int)((((color[idx2][1] - color[idx1][1]) * fractBetween) + color[idx1][1]) * 255.0);
    ib = (int)((((color[idx2][2] - color[idx1][2]) * fractBetween) + color[idx1][2]) * 255.0);

    int offset = (y * SENSOR_W + x);

    pixels[offset] = (ib << 16) | (ig << 8) | (ir << 0);
}

int main(void) {
	int minTemp = 100;
    int maxTemp = 0;

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

    static uint16_t eeMLX90641[832];
    float emissivity = 1;
    uint16_t frame[834];
    static float image[192];
    static float mlx90641To[192];
    float eTa;
    static uint16_t data[192*sizeof(float)];

    auto frame_time = std::chrono::microseconds(FRAME_TIME_MICROS + OFFSET_MICROS);

    //MLX90641_SetDeviceMode(MLX_I2C_ADDR, 0);
    //MLX90641_SetSubPageRepeat(MLX_I2C_ADDR, 0);
    switch(FPS){
        case 1:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b001);
            break;
        case 2:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b010);
            break;
        case 4:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b011);
            break;
        case 8:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b100);
            break;
        case 16:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b101);
            break;
        case 32:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b110);
            break;
        case 64:
            MLX90641_SetRefreshRate(MLX_I2C_ADDR, 0b111);
            break;
        default:
            printf("Unsupported framerate: %d", FPS);
            return 1;
    }
    // MLX90641_SetChessMode(MLX_I2C_ADDR);

    paramsMLX90641 mlx90641;
    MLX90641_DumpEE(MLX_I2C_ADDR, eeMLX90641);

    MLX90641_ExtractParameters(eeMLX90641, &mlx90641);
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
        MLX90641_GetFrameData(MLX_I2C_ADDR, frame);

        eTa = MLX90641_GetTa(frame, &mlx90641);
	
        MLX90641_CalculateTo(frame, &mlx90641, emissivity, eTa, mlx90641To);

        MLX90641_BadPixelsCorrection((&mlx90641)->brokenPixel, mlx90641To);

        minTemp = 100;
        maxTemp = 0;
        for(int i=0;i<192;i++){
            if(minTemp > mlx90641To[i]) minTemp = mlx90641To[i];
            if(maxTemp < mlx90641To[i]) maxTemp = mlx90641To[i];
        }

        for(int y = 0; y < SENSOR_W; y++){
            for(int x = 0; x < SENSOR_H; x++){
                float val = mlx90641To[SENSOR_H * (SENSOR_W-1-y) + x];
                put_pixel_false_colour(y, x, val);
            }
        }
        
	SDL_UpdateTexture(texture, NULL, (uint8_t *)pixels, SENSOR_W * sizeof(uint32_t));

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

        sprintf(text_buffer,"Min:%d°C",minTemp);
        min_temp_text = TTF_RenderUTF8_Solid( font, text_buffer, blue_color );
        sprintf(text_buffer,"Max:%d°C",maxTemp);
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
