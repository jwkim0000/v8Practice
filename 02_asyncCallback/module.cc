#include <node/node.h>
#include <iostream>
#include <thread>
#include <node/uv.h>

namespace module {
 
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Null;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Handle;
using v8::HandleScope;
using v8::Context;
using v8::Persistent;
 
void MethodHello(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world"));
}

// Direct call function of first parameter.
void MethodFunc(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // If parameter is invalid, throw exception.
  if (args.Length() < 1 || !args[0]->IsFunction()) {
    Local<String> err_msg = String::NewFromUtf8(isolate, "Invalid Parameter");
    Local<Value> exception = v8::Exception::Error(err_msg);
    isolate->ThrowException(exception);
    return;
  }

  // Call function directly
  Local<Function> cb = Local<Function>::Cast(args[0]);
  cb->Call(Null(isolate), 0, NULL);
}

static int g_timer_count = 0;
static int g_timer_max = 2;

void _timer_cb(uv_timer_t *timer)
{
  Isolate *isolate = Isolate::GetCurrent();

  // Define handle scope
  HandleScope handle_scope(isolate);
  // Retrieve persistent from uv timer handle.
  Persistent<Function> *pf = (Persistent<Function> *)timer->data;

  // Create argument to forward callback.
  Local<v8::Value> argv[1] = {
    String::NewFromUtf8(isolate, "Timer invoked")
  };

  Local<Object> global = isolate->GetCurrentContext()->Global();
  // Create Local handle from persistent handle.
  Local<Function> callback = Local<Function>::New(isolate, *pf);

  // Callback call.
  callback->Call(global, 1, argv);

  g_timer_count++;
  if (g_timer_count >= g_timer_max) {
    // Clear memory from heap.
    pf->Reset();
    delete pf;

    // Destroy timer.
    uv_timer_stop(timer);
    free(timer);
  }
}

void MethodTimer(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // If parameter is invalid, throw exception.
  if (args.Length() < 1 || !args[0]->IsFunction()) {
    Local<String> err_msg = String::NewFromUtf8(isolate, "Invalid Parameter");
    Local<Value> exception = v8::Exception::Error(err_msg);
    isolate->ThrowException(exception);
    return;
  }

  // Creates timer using libuv
  uv_loop_t *loop = uv_default_loop();
  uv_timer_t *timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
  uv_timer_init(loop, timer);

  Local<Function> cb = Local<Function>::Cast(args[0]);

  // Creates persistent handle for keeping parameter
  Persistent<Function> *pf = new Persistent<Function>(isolate, cb);
  // Save to uv timer handle.
  timer->data = (void *)pf;

  uv_timer_start(timer, _timer_cb, 1000, 1000);
}
 

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", MethodHello);
  NODE_SET_METHOD(exports, "directCall", MethodFunc);
  NODE_SET_METHOD(exports, "asyncCall", MethodTimer);
}
 
NODE_MODULE(addon, init)
 
}

