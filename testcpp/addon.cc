// addon.cc
#include <node.h>
#include <v8.h>
#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
//#include <thread>
#include <math.h>
#include "MLX90641_API.h"

namespace demo {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;
using v8::Context;
using v8::Function;
using v8::Null;


// Thermo

//s#include "MLX90641_API.h"

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

uint32_t pixels[SENSOR_W * SENSOR_H];
  static uint16_t eeMLX90641[832];
    float emissivity = 1;
    uint16_t frame[834];
    static float image[192];
    static float mlx90641To[192];
    float eTa;
    static uint16_t data[192*sizeof(float)];

    auto frame_time = std::chrono::microseconds(FRAME_TIME_MICROS + OFFSET_MICROS);

	int minTemp = 100;
    int maxTemp = 0;
        paramsMLX90641 mlx90641;

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


// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void Add(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() < 2) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,
                            "Wrong number of arguments",v8::NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  // Check the argument types
  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,
                            "Wrong arguments",v8::NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  // Perform the operation
  double value =
      args[0].As<Number>()->Value() * args[1].As<Number>()->Value();
  Local<Number> num = Number::New(isolate, value);

  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(num);
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = {
      String::NewFromUtf8(isolate,
                          "hello world",v8::NewStringType::kNormal).ToLocalChecked() };
  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}

void GetThermoImage(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  //Local<Array> argv[argc] = Array::New(isolate);

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

    Local<Array> Pixels = Array::New(isolate);
    for(int y = 0; y < SENSOR_W; y++){
            for(int x = 0; x < SENSOR_H; x++){
                int offset = (y * SENSOR_W + x);
                //Pixels[offset]=pixels[offset];
                Local<Number> pixel = Number::New(isolate, pixels[offset]);
                Pixels->Set(offset, pixel);
            }
        }
    args.GetReturnValue().Set(Pixels);
  //cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}


void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "add", Add);
  NODE_SET_METHOD(exports, "run", RunCallback);
  NODE_SET_METHOD(exports, "getThermoImage", GetThermoImage);




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
            //return 1;
    }
    // MLX90641_SetChessMode(MLX_I2C_ADDR);


    MLX90641_DumpEE(MLX_I2C_ADDR, eeMLX90641);

    MLX90641_ExtractParameters(eeMLX90641, &mlx90641);
    MLX90641_ExtractParameters(eeMLX90641, &mlx90641);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo