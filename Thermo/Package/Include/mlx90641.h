#ifndef _MLX90641_H_
#define _MLX90641_H_

#include <chrono>
//#include <thread>
#include <math.h>
#include "MLX90641_API.h"



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



class MLX90641  {
    public:
        MLX90641() ;
        int getMinTemp();
        int getMaxTemp();

        int getPixelsSize();

        uint32_t * getImage();
    protected:
        void put_pixel_false_colour(int x, int y, double v);

    public:
        uint32_t pixels[SENSOR_W * SENSOR_H];
        uint16_t eeMLX90641[832];
        float emissivity = 1;
        uint16_t frame[834];
        float image[192];
        float mlx90641To[192];
        float eTa;
        uint16_t data[192*sizeof(float)];

        //auto frame_time = std::chrono::microseconds(FRAME_TIME_MICROS + OFFSET_MICROS);

        int minTemp = 100;
        int maxTemp = 0;
        paramsMLX90641 mlx90641;
};

#endif