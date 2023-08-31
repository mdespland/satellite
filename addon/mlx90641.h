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
        MLX90641() {
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
        int getMinTemp() {
            return minTemp;
        }
        int getMaxTemp() {
            return maxTemp;
        }

        int getPixelsSize() {
            return SENSOR_W*SENSOR_H;
        }

        uint32_t * getImage() {
            auto start = std::chrono::system_clock::now();
            MLX90641_GetFrameData(MLX_I2C_ADDR, this->frame);

            eTa = MLX90641_GetTa(this->frame, &this->mlx90641);
            
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
    protected:
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

            this->pixels[offset] = (ib << 16) | (ig << 8) | (ir << 0);
        }

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