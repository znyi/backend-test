#define _USE_MATH_DEFINES

#include "lib.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <cmath>
#include <iostream>

//Init
Napi::Object cppsrc::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("sineStep1", Napi::Function::New(env, cppsrc::SineStep1Wrapped)); 
    exports.Set("sineStep2", Napi::Function::New(env, cppsrc::SineStep2Wrapped)); 
    exports.Set("sineStep3", Napi::Function::New(env, cppsrc::SineStep3Wrapped)); 
    exports.Set("sineStep4", Napi::Function::New(env, cppsrc::SineStep4Wrapped)); 
    exports.Set("sineStop", Napi::Function::New(env, cppsrc::SineStopWrapped)); 
    exports.Set("sineOutput", Napi::Function::New(env, cppsrc::SineOutputWrapped)); 
  return exports;
}

//Sine Wrapped
Napi::String cppsrc::SineStep1Wrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String inputJsonStr = info[0].As<Napi::String>();
    Napi::String returnString = Napi::String::New(env, cppsrc::sineStep1(inputJsonStr));
    return returnString;
}
Napi::String cppsrc::SineStep2Wrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String inputJsonStr = info[0].As<Napi::String>();
    Napi::String returnString = Napi::String::New(env, cppsrc::sineStep2(inputJsonStr));
    return returnString;
}
Napi::String cppsrc::SineStep3Wrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String inputJsonStr = info[0].As<Napi::String>();
    Napi::String returnString = Napi::String::New(env, cppsrc::sineStep3(inputJsonStr));
    return returnString;
}
Napi::String cppsrc::SineStep4Wrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();

    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String inputJsonStr = info[0].As<Napi::String>();
    Napi::String returnString = Napi::String::New(env, cppsrc::sineStep4(inputJsonStr));

    return returnString;
}
Napi::String cppsrc::SineStopWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    Napi::String returnString = Napi::String::New(env, cppsrc::sineStop());
    return returnString;
}
Napi::String cppsrc::SineOutputWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    if(info.Length() < 1 || !info[0].IsString()){ //error if improper args
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String inputJsonStr = info[0].As<Napi::String>();
    Napi::String returnString = Napi::String::New(env, cppsrc::sineOutput(inputJsonStr));
    return returnString;
}

//sine cpp functions
std::string cppsrc::sineStep1(std::string jsonstr){
/* example json (input)
{
    "variables": {
            "a": 2
    }
}
*/
/* example json (output)
{
    "variables": {
        "a": 2
    },
    "buffer": [
        2, 20, 1, 2, 9, 1, 1, 2
    ]
}
*/
    rapidjson::Document sineStep1Input;
    sineStep1Input.Parse(jsonstr.c_str());

    rapidjson::Document sineStep1Output(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocator = sineStep1Output.GetAllocator();
 
    rapidjson::Value& variables = sineStep1Input["variables"];
    int a = variables["a"].GetInt();
    rapidjson::Value buffer(rapidjson::kArrayType);

    uint8_t sdoInputBuffer[] = {2, 20, 1, 2, 9, 1, 1, a}; 
    for (uint8_t elem : sdoInputBuffer){
        buffer.PushBack(elem, allocator);
    }
    
    sineStep1Output.AddMember("variables", variables, allocator);
    sineStep1Output.AddMember("buffer", buffer, allocator);

    rapidjson::StringBuffer resultStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(resultStringBuffer);
    sineStep1Output.Accept(writer);
    return resultStringBuffer.GetString();
}
std::string cppsrc::sineStep2(std::string jsonstr){
/* example json (input)
{
    "variables": {
        "a": 2,
        "b": 5
    },
    "buffer": [
        2, 59, 1, 2, 9, 1, 1, 3
    ]
}
*/
/* example json (output)
{
    "variables": {
        "a": 2,
        "b": 5,
        "aPrime": 3
    },
    "buffer": [
        2, 20, 1, 2, 9, 1, 1, 5
    ]
}
*/
    rapidjson::Document sineStep2Input;
    sineStep2Input.Parse(jsonstr.c_str());

    rapidjson::Value& variables = sineStep2Input["variables"];
    rapidjson::Value bufferFromInput = sineStep2Input["buffer"].GetArray();
    
    rapidjson::Document::AllocatorType& allocatorFromInput = sineStep2Input.GetAllocator();
    
    rapidjson::Document sineStep2Output(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocatorFromOutput = sineStep2Output.GetAllocator();

    int aPrime = bufferFromInput[bufferFromInput.Size()-1].GetInt();
    variables.AddMember("aPrime", aPrime, allocatorFromInput);

    int b = variables["b"].GetInt();
    rapidjson::Value buffer(rapidjson::kArrayType);
    uint8_t sdoInputBuffer[] = {2, 20, 1, 2, 9, 1, 1, b}; 
    for (uint8_t elem : sdoInputBuffer){
        buffer.PushBack(elem, allocatorFromOutput);
    }

    sineStep2Output.AddMember("variables", variables, allocatorFromOutput);
    sineStep2Output.AddMember("buffer", buffer, allocatorFromOutput);

    rapidjson::StringBuffer resultStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(resultStringBuffer);
    sineStep2Output.Accept(writer);
    return resultStringBuffer.GetString();
}
std::string cppsrc::sineStep3(std::string jsonstr){
/* example json (input)
{
    "variables": {
        "a": 2,
        "b": 5,
        "aPrime": 3,
        "c": 8
    },
    "buffer": [
        2, 59, 1, 2, 9, 1, 1, 6
    ]
}
*/
/* example json (output)
{
    "variables": {
        "a": 2,
        "b": 5,
        "aPrime": 3,
        "c": 8,
        "bPrime": 6
    },
    "buffer": [
        2, 20, 1, 2, 9, 1, 1, 8
    ]
}
*/
    rapidjson::Document sineStep3Input;
    sineStep3Input.Parse(jsonstr.c_str());

    rapidjson::Value& variables = sineStep3Input["variables"];
    rapidjson::Value bufferFromInput = sineStep3Input["buffer"].GetArray();
    
    rapidjson::Document::AllocatorType& allocatorFromInput = sineStep3Input.GetAllocator();
    
    rapidjson::Document sineStep3Output(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocatorFromOutput = sineStep3Output.GetAllocator();

    int bPrime = bufferFromInput[bufferFromInput.Size()-1].GetInt();
    variables.AddMember("bPrime", bPrime, allocatorFromInput);

    int c = variables["c"].GetInt();
    rapidjson::Value buffer(rapidjson::kArrayType);
    uint8_t sdoInputBuffer[] = {2, 20, 1, 2, 9, 1, 1, c}; 
    for (uint8_t elem : sdoInputBuffer){
        buffer.PushBack(elem, allocatorFromOutput);
    }

    sineStep3Output.AddMember("variables", variables, allocatorFromOutput);
    sineStep3Output.AddMember("buffer", buffer, allocatorFromOutput);

    rapidjson::StringBuffer resultStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(resultStringBuffer);
    sineStep3Output.Accept(writer);
    return resultStringBuffer.GetString();
}
std::string cppsrc::sineStep4(std::string jsonstr){
/* example json (input)
{
    "variables": {
        "a": 2,
        "aPrime": 3,
        "b": 5,
        "bPrime": 6,
        "c": 8
    },
    "buffer": [
        2, 59, 1, 2, 9, 1, 1, 9
    ]
}
*/
/* example json (output)
{
    "variables": {
        "a": 2,
        "b": 5,
        "aPrime": 3,
        "c": 8,
        "bPrime": 6,
        "cPrime": 9
    },
    "buffer": [
        2, 20, 4, 2, 1, 1, 1, 1, 2, 3, 1, 1, 1, 2, 4, 1, 2, 2, 0, 2, 1, 2, 1, 1, 1, 2 
    ]
}
*/
    rapidjson::Document sineStep4Input;
    sineStep4Input.Parse(jsonstr.c_str());

    rapidjson::Value& variables = sineStep4Input["variables"];
    rapidjson::Value bufferFromInput = sineStep4Input["buffer"].GetArray();
    
    rapidjson::Document::AllocatorType& allocatorFromInput = sineStep4Input.GetAllocator();
    
    rapidjson::Document sineStep3Output(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& allocatorFromOutput = sineStep4Input.GetAllocator();

    int cPrime = bufferFromInput[bufferFromInput.Size()-1].GetInt();
    variables.AddMember("cPrime", cPrime, allocatorFromInput);

    rapidjson::Value buffer(rapidjson::kArrayType);
    uint8_t pdoInputBuffer[] = {
        2, 20, 4, 
        2, 1, 1, 1, 1, 
        2, 3, 1, 1, 1, 
        2, 4, 1, 2, 2, 0, 2, 1, 
        2, 1, 1, 1, 2 }; 
    for (uint8_t elem : pdoInputBuffer){
        buffer.PushBack(elem, allocatorFromOutput);
    }

    sineStep3Output.AddMember("variables", variables, allocatorFromOutput);
    sineStep3Output.AddMember("buffer", buffer, allocatorFromOutput);

    rapidjson::StringBuffer resultStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(resultStringBuffer);
    sineStep3Output.Accept(writer);
    return resultStringBuffer.GetString();
}
std::string cppsrc::sineStop(){
/* example json (output)
    (no input)
*/
/* example json (output)
[
    2, 20, 1, 2, 1, 1, 1, 1
]
*/
    rapidjson::Document sineStopOutput(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocator = sineStopOutput.GetAllocator();

    uint8_t pdoInputBuffer[] = { 
        2, 20, 1, 
        2, 1, 1, 1, 1 }; 
    for (uint8_t elem : pdoInputBuffer){
        sineStopOutput.PushBack(elem, allocator);
    }
 
    rapidjson::StringBuffer resultStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(resultStringBuffer);
    sineStopOutput.Accept(writer);
    return resultStringBuffer.GetString();
}
std::string cppsrc::sineOutput(std::string jsonstr){
/* example json (input)
{
    "variables": {
        "a": 2,
        "aPrime": 3,
        "b": 5,
        "bPrime": 6,
        "c": 8,
        "cPrime": 9
    },
    "chunk": [
        [3,49,2,2,0,0,128,82,68,2,1,0,128,82,196],
        [3,49,2,2,0,0,128,82,68,2,1,0,128,82,196],
        [3,49,2,2,0,0,128,82,68,2,1,0,128,82,196]
    ]
}
*/
/* example json (output)
[
    {
        "t_deg": 842,
        "t_rad": 14.695672035217285,
        "y": -0.4033072888851166
    },
    {
        "t_deg": 842,
        "t_rad": 14.695672035217285,
        "y": -0.4033072888851166
    },
    {
        "t_deg": 842,
        "t_rad": 14.695672035217285,
        "y": -0.4033072888851166
    }
*/
    rapidjson::Document sineInput;
    sineInput.Parse(jsonstr.c_str());

    rapidjson::Value variables = sineInput["variables"].GetObject();
    int a = variables["a"].GetInt();
    int b = variables["b"].GetInt();
    int c = variables["c"].GetInt();
    
    auto chunk = sineInput["chunk"].GetArray();

    rapidjson::Document sineOutputChunk(rapidjson::kArrayType);
    rapidjson::Document::AllocatorType& allocatorFromOutput = sineOutputChunk.GetAllocator();
    
    for(auto& lineArray : chunk){
        rapidjson::Document lineOutput(rapidjson::kObjectType);
        rapidjson::Document::AllocatorType& allocatorLineOutput = lineOutput.GetAllocator();
 
        auto line = lineArray.GetArray();

        std::vector<uint8_t> lineBuffer;
        for(auto& value : line){
            lineBuffer.push_back(value.GetUint());
        }

        float t_deg;
        memcpy(&t_deg, &lineBuffer[5], 4);

        float t_rad = (t_deg / 180.0f) * M_PI;

        float y = a * sin((b * t_rad) + c);

        lineOutput.AddMember("t_deg", t_deg, allocatorFromOutput);
        lineOutput.AddMember("t_rad", t_rad, allocatorFromOutput);
        lineOutput.AddMember("y", y, allocatorFromOutput);

        sineOutputChunk.PushBack(lineOutput, allocatorFromOutput);
    }

    rapidjson::StringBuffer resultStringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(resultStringBuffer);
    sineOutputChunk.Accept(writer);
    return resultStringBuffer.GetString();
}