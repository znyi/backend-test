#include "lib.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <cmath>
#include <iostream>

std::string cppsrc::getSdoCommand(std::string jsonstr){
    rapidjson::Document doc;
    doc.Parse(jsonstr.c_str());

    if (!doc.IsNumber()) {
        std::cerr << "Invalid JSON document or not a number." << std::endl;
        return "error in getSdoCommand - invalid argument";
    }

    int a = doc.GetInt(); 

    rapidjson::Document sdoCommand(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = sdoCommand.GetAllocator();

    uint8_t sdoInputBuffer[] = {2, 20, 4, 2, 9, 1, 1, a}; 

    for (uint8_t elem : sdoInputBuffer){
        sdoCommand.PushBack(elem, allocator);
    }
 
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    sdoCommand.Accept(writer);

    return buffer.GetString();
}

Napi::String cppsrc::getSdoCommandWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    Napi::String inputJsonStr = info[0].As<Napi::String>();

    Napi::String returnString = Napi::String::New(env, cppsrc::getSdoCommand(inputJsonStr));
    
    return returnString;
}

std::string cppsrc::getSdoOutput(std::string jsonstr){
    rapidjson::Document doc;
    doc.Parse(jsonstr.c_str());

    if (!doc.IsArray()) {
        std::cerr << "Invalid JSON document or not an array." << std::endl;
        return "error in getSdoOutput - invalid argument";
    }

    rapidjson::Document sdoOutput(rapidjson::kNumberType);
    rapidjson::Document::AllocatorType& allocator = sdoOutput.GetAllocator();
 
    sdoOutput.SetUint(doc[doc.Size()-1].GetUint()); //take last elem

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    sdoOutput.Accept(writer);

    return buffer.GetString();
}

Napi::String cppsrc::getSdoOutputWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    Napi::String inputJsonStr = info[0].As<Napi::String>();

    Napi::String returnString = Napi::String::New(env, cppsrc::getSdoOutput(inputJsonStr));
    
    return returnString;
}

Napi::Object cppsrc::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getSdoCommand", Napi::Function::New(env, cppsrc::getSdoCommandWrapped)); 
    exports.Set("getSdoOutput", Napi::Function::New(env, cppsrc::getSdoOutputWrapped)); 
  return exports;
}