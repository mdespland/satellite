#include "napi.h"
#include "mlx90641.h"

class DoHeavyMathWorker : public Napi::AsyncWorker {
 public:
  DoHeavyMathWorker(const Napi::Env& env, uint32_t num_1, uint32_t num_2)
      : Napi::AsyncWorker{env, "DoHeavyMathWorker"},
        m_deferred{env},
        m_num_1{num_1},
        m_num_2{num_2} {
          this->thermoCamera=new MLX90641();
        }

  /**
   * GetPromise associated with _deferred for return to JS
   */
  Napi::Promise GetPromise() { return m_deferred.Promise(); }

 protected:
  /**
   * Simulate heavy math work
   */
  void Execute() {
    uint32_t * tmp=this->thermoCamera->getImage();
    for (int i=0;i<this->thermoCamera->getPixelsSize();i++) m_result[i]=tmp[i];
  }

  /**
   * Resolve the promise with the result
   */
  //void OnOK() { m_deferred.Resolve(Napi::Number::New(Env(), m_result)); }
  void OnOK() {
    Napi::Array array=Napi::Array::New(Env(),  SENSOR_W * SENSOR_H);
    for (int i=0;i<SENSOR_W * SENSOR_H;i++) array[i]=Napi::Number::New(Env(), m_result[i]) ;
    m_deferred.Resolve(array); 
}

  /**
   * Reject the promise with errors
   */
  void OnError(const Napi::Error& err) { m_deferred.Reject(err.Value()); }

 private:
  Napi::Promise::Deferred m_deferred;
  uint32_t m_num_1;
  uint32_t m_num_2;
  //uint32_t m_result;
  uint32_t m_result[SENSOR_W * SENSOR_H];
  MLX90641 * thermoCamera;
};