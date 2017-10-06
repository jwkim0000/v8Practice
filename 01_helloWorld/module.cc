#include <node/node.h> // 중요 1. header
#include <iostream>
 
namespace demo {
 
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
 
void Method1(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}
 
void Method2(const FunctionCallbackInfo<Value>& args) {
  if (args.Length() != 2) {
    std::cout<<"Invalid Parameters"<<std::endl;
    return;
  }
  if (args[0]->IsNumber() && args[1]->IsNumber()) {
    double value = args[0]->NumberValue() + args[1]->NumberValue();
    Isolate* isolate = args.GetIsolate();
    Local<Number> num = Number::New(isolate, value);
    args.GetReturnValue().Set(num);
  }
}
 
void init(Local<Object> exports) { // 중요 3. Method 정의
  NODE_SET_METHOD(exports, "hello", Method1);
  NODE_SET_METHOD(exports, "sum", Method2);
}
 
NODE_MODULE(addon, init) // 중요 2. 모듈 정의
 
}

