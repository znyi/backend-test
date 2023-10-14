#include "lib.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <cmath>
#include <iostream>

bool angel::isPrime(int n){
      if(n == 1 || n == 0) return false;
      if(n == 2) return true;
      if (n % 2 == 0) return false;
      
      for(int i = 3; i <= std::sqrt(n); i += 2){
        if(n % i == 0) return false;
      }
      return true;
}

std::string angel::getPrimes(std::string jsonstr){
    rapidjson::Document doc;
    doc.Parse(jsonstr.c_str());

    rapidjson::Value& startRange = doc["startRange"];
    rapidjson::Value& endRange = doc["endRange"];

    rapidjson::Document primeList(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = primeList.GetAllocator();

    for(int i = startRange.GetInt(); i <= endRange.GetInt(); i++){
        if(isPrime(i)){
            primeList.PushBack(i, allocator);
        }
    }
 
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    primeList.Accept(writer);

    return buffer.GetString();
}

Napi::String angel::getPrimesWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    Napi::String inputJsonStr = info[0].As<Napi::String>();

    Napi::String returnString = Napi::String::New(env, angel::getPrimes(inputJsonStr));
    
    return returnString;
}

Napi::Object angel::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getPrimes", Napi::Function::New(env, angel::getPrimesWrapped)); 
  return exports;
}