#include "napi.h"
#include "worker.h"


Napi::Value Capture(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  /*if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "num1 must be a number")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }
  uint32_t num_1 = info[0].As<Napi::Number>().Uint32Value();
  
  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "num2 must be a number")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }
  uint32_t num_2 = info[1].As<Napi::Number>().Uint32Value();*/
  
  MLX90641Worker* worker = new MLX90641Worker(env);
  worker->Queue();
  return worker->GetPromise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "capture"),
              Napi::Function::New(env, Capture));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)