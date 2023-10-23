#include <napi.h>
namespace cppsrc {
  Napi::Object Init(Napi::Env env, Napi::Object exports);

  std::string getSdoCommand(std::string jsonstr);
  Napi::String getSdoCommandWrapped(const Napi::CallbackInfo& info);
  std::string getSdoOutput(std::string jsonstr);
  Napi::String getSdoOutputWrapped(const Napi::CallbackInfo& info);

  std::string getPdoExecCommand();
  Napi::String getPdoExecCommandWrapped(const Napi::CallbackInfo& info);
  std::string getPdoStopCommand();
  Napi::String getPdoStopCommandWrapped(const Napi::CallbackInfo& info);
  std::string getPdoOutput(std::string jsonstr);
  Napi::String getPdoOutputWrapped(const Napi::CallbackInfo& info);
}