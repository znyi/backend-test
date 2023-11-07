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

    uint8_t sdoInputBuffer[] = {2, 20, 1, 2, 9, 1, 1, a}; 

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

std::string cppsrc::getPdoExecCommand(){
    rapidjson::Document pdoCommand(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = pdoCommand.GetAllocator();

    uint8_t pdoInputBuffer[] = {
        2, 20, 4, 
        2, 1, 1, 1, 1, 
        2, 3, 1, 1, 1, 
        2, 4, 1, 2, 2, 0, 2, 1, 
        2, 1, 1, 1, 2 }; 

    for (uint8_t elem : pdoInputBuffer){
        pdoCommand.PushBack(elem, allocator);
    }
 
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    pdoCommand.Accept(writer);

    return buffer.GetString();
}

Napi::String cppsrc::getPdoExecCommandWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    Napi::String returnString = Napi::String::New(env, cppsrc::getPdoExecCommand());
    
    return returnString;
}
std::string cppsrc::getPdoStopCommand(){
    rapidjson::Document pdoCommand(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = pdoCommand.GetAllocator();

    uint8_t pdoInputBuffer[] = { 
        2, 20, 1, 
        2, 1, 1, 1, 1 }; 

    for (uint8_t elem : pdoInputBuffer){
        pdoCommand.PushBack(elem, allocator);
    }
 
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    pdoCommand.Accept(writer);

    return buffer.GetString();
}

Napi::String cppsrc::getPdoStopCommandWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    Napi::String returnString = Napi::String::New(env, cppsrc::getPdoStopCommand());
    
    return returnString;
}

std::string cppsrc::getPdoOutput(std::string jsonstr){
    rapidjson::Document doc;
    doc.Parse(jsonstr.c_str());

    if (!doc.IsArray()) {
        std::cerr << "Invalid JSON document or not an array." << std::endl;
        return "error in getPdoOutput - invalid argument";
    }

    rapidjson::Document pdoOutput(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = pdoOutput.GetAllocator();
 
    auto docArray = doc.GetArray();
    std::vector<uint8_t> pdoOutputBuffer;

    for(auto& value : docArray){
        pdoOutputBuffer.push_back(value.GetUint());
    }
    
    float f1;
    memcpy(&f1, &pdoOutputBuffer[5], 4);
    float f2;
    memcpy(&f2, &pdoOutputBuffer[11], 4);

    pdoOutput.AddMember("data1", f1, allocator);
    pdoOutput.AddMember("data2", f2, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    pdoOutput.Accept(writer);

    return buffer.GetString();
}

Napi::String cppsrc::getPdoOutputWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }

    Napi::String inputJsonStr = info[0].As<Napi::String>();

    Napi::String returnString = Napi::String::New(env, cppsrc::getPdoOutput(inputJsonStr));
    
    return returnString;
}

Napi::Object cppsrc::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getSdoCommand", Napi::Function::New(env, cppsrc::getSdoCommandWrapped)); 
    exports.Set("getSdoOutput", Napi::Function::New(env, cppsrc::getSdoOutputWrapped)); 
    exports.Set("getPdoExecCommand", Napi::Function::New(env, cppsrc::getPdoExecCommandWrapped)); 
    exports.Set("getPdoStopCommand", Napi::Function::New(env, cppsrc::getPdoStopCommandWrapped)); 
    exports.Set("getPdoOutput", Napi::Function::New(env, cppsrc::getPdoOutputWrapped)); 
  return exports;
}