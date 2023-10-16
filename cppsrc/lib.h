#include <napi.h>
namespace cppsrc {
  Napi::Object Init(Napi::Env env, Napi::Object exports);

  bool isPrime(int n);
  std::string getPrimes(std::string jsonstr);
  Napi::String getPrimesWrapped(const Napi::CallbackInfo& info);
}