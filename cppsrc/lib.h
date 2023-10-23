#include <napi.h>
namespace cppsrc {
  Napi::Object Init(Napi::Env env, Napi::Object exports);

  std::string getSdoCommand(std::string jsonstr);
  Napi::String getSdoCommandWrapped(const Napi::CallbackInfo& info);
  std::string getSdoOutput(std::string jsonstr);
  Napi::String getSdoOutputWrapped(const Napi::CallbackInfo& info);
}