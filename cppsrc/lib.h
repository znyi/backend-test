#include <napi.h>
namespace cppsrc {
  Napi::Object Init(Napi::Env env, Napi::Object exports);

  Napi::String SineStep1Wrapped(const Napi::CallbackInfo& info);
  Napi::String SineStep2Wrapped(const Napi::CallbackInfo& info);
  Napi::String SineStep3Wrapped(const Napi::CallbackInfo& info);
  Napi::String SineStep4Wrapped(const Napi::CallbackInfo& info);
  Napi::String SineStopWrapped(const Napi::CallbackInfo& info);
  Napi::String SineOutputWrapped(const Napi::CallbackInfo& info);

  std::string sineStep1(std::string jsonstr);
  std::string sineStep2(std::string jsonstr);
  std::string sineStep3(std::string jsonstr);
  std::string sineStep4(std::string jsonstr);
  std::string sineStop();
  std::string sineOutput(std::string jsonstr);

}