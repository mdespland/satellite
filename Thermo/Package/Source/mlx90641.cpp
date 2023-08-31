#include "mlx90641.h"

#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>



MLX90641::MLX90641() {
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
                    //return 1;
    }
    // MLX90641_SetChessMode(MLX_I2C_ADDR);


    MLX90641_DumpEE(MLX_I2C_ADDR, this->eeMLX90641);

    MLX90641_ExtractParameters(this->eeMLX90641, &this->mlx90641);
    MLX90641_ExtractParameters(this->eeMLX90641, &this->mlx90641);
}
int MLX90641::getMinTemp() {
    return this->minTemp;
}
int MLX90641::getMaxTemp() {
    return this->maxTemp;
}

int MLX90641::getPixelsSize() {
    return SENSOR_W*SENSOR_H;
}

uint32_t * MLX90641::getImage() {
    auto start = std::chrono::system_clock::now();
    MLX90641_GetFrameData(MLX_I2C_ADDR, this->frame);

    this->eTa = MLX90641_GetTa(this->frame, &this->mlx90641);
            
    MLX90641_CalculateTo(this->frame, &this->mlx90641, this->emissivity, this->eTa, this->mlx90641To);

    MLX90641_BadPixelsCorrection((&this->mlx90641)->brokenPixel, this->mlx90641To);

    this->minTemp = 100;
    this->maxTemp = 0;
    for(int i=0;i<192;i++){
        if(this->minTemp > this->mlx90641To[i]) this->minTemp = this->mlx90641To[i];
        if(this->maxTemp < this->mlx90641To[i]) this->maxTemp = this->mlx90641To[i];
    }

    for(int y = 0; y < SENSOR_W; y++){
        for(int x = 0; x < SENSOR_H; x++){
            float val = this->mlx90641To[SENSOR_H * (SENSOR_W-1-y) + x];
            this->put_pixel_false_colour(y, x, val);
        }
    }
    return this->pixels;

}
void MLX90641::put_pixel_false_colour(int x, int y, double v) {
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

    this->pixels[offset] = (ib << 16) | (ig << 8) | (ir << 0);
}