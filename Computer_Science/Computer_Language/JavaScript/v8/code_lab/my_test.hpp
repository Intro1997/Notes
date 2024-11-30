#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "include/libplatform/libplatform.h"
#include "include/v8-container.h"
#include "include/v8-context.h"
#include "include/v8-exception.h"
#include "include/v8-external.h"
#include "include/v8-initialization.h"
#include "include/v8-isolate.h"
#include "include/v8-local-handle.h"
#include "include/v8-primitive.h"
#include "include/v8-script.h"
#include "include/v8-template.h"
#include "include/v8.h"
#include "src/debug/interface-types.h"

// all Test functions declare
void TestLocker(v8::Isolate* isolate);

void TestArrayBufferAllocator(int argc, char* argv[]);

void ContextScopeTest(v8::Isolate* isolate);

void IsolateScopeTest(v8::Isolate* isolate);

void GlobalPropertyTest(v8::Isolate* isolate);

void ObjectAccessorTest(v8::Isolate* isolate);

void ConnectCppObjectWithJsObject(v8::Isolate* isolate);

void TryInterceptor(v8::Isolate* isolate);

void GetArrayFromFuncReturn(v8::Isolate* isolate);

void TestInherit(v8::Isolate* isolate);

// help functions declare
void PrintXPropertyInTimes(const v8::FunctionCallbackInfo<v8::Value>& info);

void DoAdditionLoop(const v8::FunctionCallbackInfo<v8::Value>& info);

void logCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

void member_TOM_name_getter(v8::Local<v8::String> property,
                            const v8::PropertyCallbackInfo<v8::Value>& info);

void member_TOM_name_setter(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void>& info);

void PointXattrGetter(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value>& info);
void PointXattrSetter(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void>& info);
void PointYattrGetter(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value>& info);
void PointYattrSetter(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void>& info);
void PointNamedAttrGetter(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value>& info);
void PointNamedAttrSetter(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<v8::Value>& info);

v8::Local<v8::Array> Create3DCoordinate(int x, int y, int z);

void OperatingSystemConstructor(
    const v8::FunctionCallbackInfo<v8::Value>& info) {
  std::cout << "get in cpp OperatingSystemConstructor\n";
  info.GetReturnValue().Set(info.This());
}

void WindowsConstructor(const v8::FunctionCallbackInfo<v8::Value>& info) {
  std::cout << "get in cpp WindowsConstructor\n";
  info.GetReturnValue().Set(info.This());
}

void macOSConstructor(const v8::FunctionCallbackInfo<v8::Value>& info) {
  std::cout << "get in cpp macOSConstructor\n";
  info.GetReturnValue().Set(info.This());
}

// helper data struct
#define FINISH_TOTAL 2
uint32_t finish_cnt = 0;
std::mutex finish_cnt_lock;

class Point {
 public:
  Point(int x, int y) : x_(x), y_(y) {
    std::cout << "get in cpp Point class constructor\n";
  }
  int x_, y_;
};

class MyArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  void* Allocate(size_t length) override {
    void* data = calloc(length, 1);
    std::cout << "ArrayBuffer Allocate data: " << data << " length:" << length
              << "\n";
    return data;
  }

  void* AllocateUninitialized(size_t length) override {
    void* data = malloc(length);
    return data;
  }

  void Free(void* data, size_t length) override {
    std::cout << "ArrayBuffer Free data: " << data << " length:" << length
              << "\n";
    free(data);
  }
};
// test function implementation
void TestLocker(v8::Isolate* isolate) {
  {
    v8::HandleScope handle_scope(isolate);
    v8::Isolate::Scope isolate_scope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Function> doAdditionLoop_func =
        v8::Function::New(context, DoAdditionLoop).ToLocalChecked();
    v8::Local<v8::Object> global = context->Global();
    if (!global
             ->Set(context,
                   v8::String::NewFromUtf8Literal(isolate, "do_addition_loop"),
                   doAdditionLoop_func)
             .ToChecked()) {
      std::cerr << "Set function DoAdditionLoop to global object failed\n";
      return;
    }

    v8::Local<v8::Function> printXPropertyInTimes_func =
        v8::Function::New(context, PrintXPropertyInTimes).ToLocalChecked();
    if (!global
             ->Set(context,
                   v8::String::NewFromUtf8Literal(isolate,
                                                  "print_x_property_in_times"),
                   printXPropertyInTimes_func)
             .ToChecked()) {
      std::cerr
          << "Set function PrintXPropertyInTimes to global object failed\n";
      return;
    }

    std::string source_code_str =
        "let a = { x : 1 }; do_addition_loop(a);"
        "print_x_property_in_times(a, 500, 5);";
    v8::Local<v8::Script> script_obj =
        v8::Script::Compile(
            context, v8::String::NewFromUtf8(isolate, source_code_str.c_str())
                         .ToLocalChecked())
            .ToLocalChecked();
    v8::MaybeLocal<v8::Value> ret = script_obj->Run(context);
    if (ret.IsEmpty()) {
      v8::TryCatch try_catch(isolate);
      v8::String::Utf8Value message(isolate, try_catch.Exception());
      std::cerr << "Exception: " << *message << std::endl;
      return;
    }

    v8::String::Utf8Value ret_str =
        v8::String::Utf8Value(isolate, ret.ToLocalChecked());
    std::cout << *ret_str << "\n";
  }

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::lock_guard<std::mutex> guard(finish_cnt_lock);
    if (finish_cnt >= FINISH_TOTAL) {
      break;
    }
  }
}

void TestArrayBufferAllocator(int argc, char* argv[]) {
  // Initialize V8.
  // v8::V8::InitializeICUDefaultLocation(argv[0]);
  // v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = new MyArrayBufferAllocator();
  v8::Isolate* isolate = v8::Isolate::New(create_params);
  {
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);

    std::string source_code_str =
        "const buf1 = new ArrayBuffer(8); const buf2 = new ArrayBuffer(1024);";
    v8::Local<v8::String> source_code =
        v8::String::NewFromUtf8(isolate, source_code_str.c_str())
            .ToLocalChecked();
    v8::Local<v8::Script> source_script =
        v8::Script::Compile(context, source_code).ToLocalChecked();
    v8::Local<v8::Value> ret_value =
        source_script->Run(context).ToLocalChecked();
    if (ret_value.IsEmpty()) {
      v8::TryCatch trycatch(isolate);
      v8::Local<v8::Value> message = trycatch.Exception();
      std::cout << "Js exception: "
                << *(v8::String::Utf8Value(isolate, message)) << "\n";
      return;
    }
    v8::String::Utf8Value ret_string(isolate, ret_value);
    std::cout << *ret_string << "\n";
  }
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::DisposePlatform();
  delete create_params.array_buffer_allocator;
}

void ContextScopeTest(v8::Isolate* isolate) {  // check if isolate is released
                                               // when destruct isolate::scope
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    global->Set(v8::String::NewFromUtf8Literal(isolate, "log"),
                v8::FunctionTemplate::New(isolate, logCallback));
    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);
    // Enter the context for compiling and running the hello world script.
    {
      v8::Context::Scope context_scope(context);

      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8Literal(isolate, "log()");

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }
    std::cout << "re-create context scope\n";
    {
      v8::Context::Scope context_scope(context);

      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8Literal(isolate, "log()");

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }
  }
}

void IsolateScopeTest(v8::Isolate* isolate) {
  // check if isolate is released when destruct isolate::scope
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    global->Set(v8::String::NewFromUtf8Literal(isolate, "log"),
                v8::FunctionTemplate::New(isolate, logCallback));

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);

    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);

    {
      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8Literal(isolate, "log()");

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }
  }
  std::cout << "re-create isolate_scope\n";
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    // Create a new context.
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    if (context.IsEmpty()) {
      std::cerr << "GetCurrentContext failed, context of isolate is deleted!\n";
      return;
    }
    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);
    {
      // Create a string containing the JavaScript source code.

      v8::Local<v8::String> source =
          v8::String::NewFromUtf8Literal(isolate, "log()");
      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();
      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);

      printf("%s\n", *utf8);
    }
  }
}

void TestInherit(v8::Isolate* isolate) {
  v8::Isolate::Scope isolate_scope(isolate);

  v8::HandleScope handle_scope(isolate);

  v8::Local<v8::FunctionTemplate> operating_system_contstructor =
      v8::FunctionTemplate::New(isolate, OperatingSystemConstructor);
  operating_system_contstructor->PrototypeTemplate()->Set(
      v8::String::NewFromUtf8Literal(isolate, "Terminal"),
      v8::String::NewFromUtf8Literal(isolate, "Basic operating system shell"));

  v8::Local<v8::FunctionTemplate> windows_os_contstructor =
      v8::FunctionTemplate::New(isolate, WindowsConstructor);
  windows_os_contstructor->PrototypeTemplate()->Set(
      v8::String::NewFromUtf8Literal(isolate, "Graphic"),
      v8::String::NewFromUtf8Literal(isolate, "Windows Direct3D"));
  windows_os_contstructor->Inherit(operating_system_contstructor);

  v8::Local<v8::FunctionTemplate> macos_os_contstructor =
      v8::FunctionTemplate::New(isolate, macOSConstructor);
  macos_os_contstructor->PrototypeTemplate()->Set(
      v8::String::NewFromUtf8Literal(isolate, "Graphic"),
      v8::String::NewFromUtf8Literal(isolate, "Apple Metal"));
  macos_os_contstructor->Inherit(operating_system_contstructor);

  v8::Local<v8::ObjectTemplate> global_tempate =
      v8::ObjectTemplate::New(isolate);
  global_tempate->Set(
      v8::String::NewFromUtf8Literal(isolate, "OperatingSystem"),
      operating_system_contstructor);
  global_tempate->Set(v8::String::NewFromUtf8Literal(isolate, "Windows"),
                      windows_os_contstructor);
  global_tempate->Set(v8::String::NewFromUtf8Literal(isolate, "macOS"),
                      macos_os_contstructor);

  v8::Local<v8::Context> context =
      v8::Context::New(isolate, nullptr, global_tempate);
  v8::Context::Scope context_scope(context);

  std::string source_code =
      "const obj = new OperatingSystem();"
      "const win = new Windows();"
      "const mac = new macOS();"
      "[[obj.Terminal, win.Terminal, mac.Terminal], win.Graphic, mac.Graphic]";
  v8::Local<v8::Script> script =
      v8::Script::Compile(context,
                          v8::String::NewFromUtf8(isolate, source_code.c_str())
                              .ToLocalChecked())
          .ToLocalChecked();
  v8::MaybeLocal<v8::Value> may_result = script->Run(context);
  if (may_result.IsEmpty()) {
    v8::TryCatch trycatch(isolate);
    v8::Local<v8::Value> message = trycatch.Exception();
    std::cout << "Js exception: " << *(v8::String::Utf8Value(isolate, message))
              << "\n";
    return;
  }

  std::cout << *(v8::String::Utf8Value(isolate, may_result.ToLocalChecked()))
            << "\n";
}

void TryExceptionInJs(v8::Isolate* isolate) {
  v8::Isolate::Scope isolate_scope(isolate);

  v8::TryCatch trycatch(isolate);

  v8::HandleScope handle_scope(isolate);

  v8::Local<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);

  std::string source_code = "my_point";
  v8::Local<v8::String> script_source_code =
      v8::String::NewFromUtf8(isolate, source_code.c_str()).ToLocalChecked();
  v8::Local<v8::Script> script_obj =
      v8::Script::Compile(context, script_source_code).ToLocalChecked();

  v8::MaybeLocal<v8::Value> maybe_result = script_obj->Run(context);

  if (maybe_result.IsEmpty()) {
    v8::Local<v8::Value> exception = trycatch.Exception();
    v8::String::Utf8Value message(isolate, exception);
    std::cout << "Exception: " << *message << "\n";
  } else {
    std::cout << *(v8::String::Utf8Value(isolate,
                                         maybe_result.ToLocalChecked()))
              << "\n";
  }
}

void GetArrayFromFuncReturn(v8::Isolate* isolate) {
  v8::Isolate::Scope isolate_scope(isolate);

  v8::HandleScope handle_scope(isolate);

  v8::Local<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);

  v8::Local<v8::Array> array_obj = Create3DCoordinate(111, 222, 333);

  if (array_obj.IsEmpty()) {
    std::cerr << "Create js array failed!\n";
    return;
  }

  v8::Local<v8::Object> global_obj = context->Global();
  if (!global_obj
           ->Set(context,
                 v8::String::NewFromUtf8Literal(isolate, "my_3d_vector"),
                 array_obj)
           .ToChecked()) {
    std::cerr << "set 3d vector failed\n";
    return;
  }

  std::string source_code = "my_3d_vector";
  v8::Local<v8::String> script_source_code =
      v8::String::NewFromUtf8(isolate, source_code.c_str()).ToLocalChecked();
  v8::Local<v8::Script> script_obj =
      v8::Script::Compile(context, script_source_code).ToLocalChecked();

  v8::String::Utf8Value result(isolate,
                               script_obj->Run(context).ToLocalChecked());
  std::cout << *result << '\n';
}

void TryInterceptor(v8::Isolate* isolate) {
  v8::HandleScope handle_scope(isolate);

  v8::Local<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);

  v8::Local<v8::ObjectTemplate> point_template =
      v8::ObjectTemplate::New(isolate);
  point_template->SetHandler(v8::NamedPropertyHandlerConfiguration(
      PointNamedAttrGetter, PointNamedAttrSetter));
  point_template->SetAccessor(v8::String::NewFromUtf8Literal(isolate, "x"),
                              PointXattrGetter, PointXattrSetter);
  point_template->SetInternalFieldCount(1);

  std::unique_ptr<Point> unique_point = std::make_unique<Point>(1, 2);
  v8::Local<v8::Object> point_obj =
      point_template->NewInstance(context).ToLocalChecked();
  point_obj->SetInternalField(0,
                              v8::External::New(isolate, unique_point.get()));

  v8::Local<v8::Object> global_obj = context->Global();
  if (!global_obj
           ->Set(context, v8::String::NewFromUtf8Literal(isolate, "cpp_point"),
                 point_obj)
           .ToChecked()) {
    std::cerr
        << "set 'cpp_point' attribute to v8 context global object failed\n";
    return;
  }

  std::string script_source_code = "cpp_point.x;";
  v8::Local<v8::String> script_source_code_v8 =
      v8::String::NewFromUtf8(isolate, script_source_code.c_str())
          .ToLocalChecked();
  v8::Local<v8::Script> script_obj =
      v8::Script::Compile(context, script_source_code_v8).ToLocalChecked();
  v8::String::Utf8Value result(isolate,
                               script_obj->Run(context).ToLocalChecked());

  std::cout << *result << std::endl;
}

void ConnectCppObjectWithJsObject(v8::Isolate* isolate) {
  {
    v8::Isolate::Scope isoloate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);

    v8::Local<v8::ObjectTemplate> point_template =
        v8::ObjectTemplate::New(isolate);
    point_template->SetInternalFieldCount(1);
    point_template->SetAccessor(
        v8::String::NewFromUtf8(isolate, "x").ToLocalChecked(),
        PointXattrGetter, PointXattrSetter);
    point_template->SetAccessor(
        v8::String::NewFromUtf8(isolate, "y").ToLocalChecked(),
        PointYattrGetter, PointYattrSetter);

    v8::Local<v8::Object> point_v8_obj =
        point_template->NewInstance(context).ToLocalChecked();
    std::unique_ptr<Point> unique_point = std::make_unique<Point>(1, 2);

    point_v8_obj->SetInternalField(
        0, v8::External::New(isolate, unique_point.get()));

    v8::Local<v8::Object> context_global = context->Global();

    if (!context_global
             ->Set(
                 context,
                 v8::String::NewFromUtf8(isolate, "cpp_point").ToLocalChecked(),
                 point_v8_obj)
             .ToChecked()) {
      std::cerr << "context_global set cpp_point object failed\n";
      return;
    }

    std::string js_source = "cpp_point.x; cpp_point.x = 666; cpp_point.x;";
    v8::Local<v8::Script> script =
        v8::Script::Compile(context,
                            v8::String::NewFromUtf8(isolate, js_source.c_str())
                                .ToLocalChecked())
            .ToLocalChecked();
    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    v8::String::Utf8Value result_string(isolate, result);
    std::cout << *result_string << std::endl;
  }
}

void ObjectAccessorTest(v8::Isolate* isolate) {
  {
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    v8::Local<v8::ObjectTemplate> member_TOM = v8::ObjectTemplate::New(isolate);

    global->Set(v8::String::NewFromUtf8Literal(isolate, "member_TOM"),
                member_TOM);

    member_TOM->SetAccessor(v8::String::NewFromUtf8Literal(isolate, "name"),
                            member_TOM_name_getter,  // getter
                            member_TOM_name_setter   // setter
    );

    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);

    v8::Context::Scope context_scope(context);

    const std::string js_source_code = "member_TOM.name = 123";

    v8::Local<v8::String> js_source_code_v8_str =
        v8::String::NewFromUtf8(isolate, js_source_code.c_str())
            .ToLocalChecked();

    v8::Local<v8::Script> script =
        v8::Script::Compile(context, js_source_code_v8_str).ToLocalChecked();

    v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    v8::String::Utf8Value result_utf8_value(isolate, result);
    std::cout << *result_utf8_value << std::endl;
  }
}

void GlobalPropertyTest(v8::Isolate* isolate) {
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    global->Set(v8::String::NewFromUtf8Literal(isolate, "log"),
                v8::FunctionTemplate::New(isolate, logCallback));

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);

    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);

    {
      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8Literal(isolate, "import * from 'hello'");

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);
    }
  }
}

v8::Persistent<v8::Object> save_obj;
v8::Persistent<v8::Object> print_help_save_obj;

void PrintXPropertyInTimes(const v8::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() < 3 || info[0]->IsNull() || !info[0]->IsObject() ||
      !info[1]->IsNumber() || !info[2]->IsNumber()) {
    std::cout
        << "Error: invalid paramater! Should be (object, number, number)\n";
    return;
  }

  {
    v8::Isolate* isolate = info.GetIsolate();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    print_help_save_obj.Reset(isolate,
                              info[0]->ToObject(context).ToLocalChecked());
    int32_t interval_time = info[1]->Int32Value(context).ToChecked();
    int32_t print_times = info[2]->Int32Value(context).ToChecked();

    if (interval_time < 0) {
      std::cerr << "cannot wait in negative time!\n";
      return;
    }
    if (print_times < 1) {
      std::cerr << "cannot print less than 1 times!\n";
      return;
    }
    // std::cout << "value = " << value << ", interval_time = " << interval_time
    //           << ", print_times = " << print_times << std::endl;

    std::thread t1(
        [](v8::Isolate* isolate, int32_t interval_time, int32_t print_times) {
          while (print_times-- > 0) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(interval_time));
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolate_scope(isolate);
            v8::HandleScope handle_scope(isolate);
            v8::Local<v8::Object> target =
                v8::Local<v8::Object>::New(isolate, print_help_save_obj);
            if (target.IsEmpty()) {
              std::cerr << "Error: get an empty saved_object\n";
              break;
            }

            v8::Local<v8::String> key =
                v8::String::NewFromUtf8Literal(isolate, "x");
            v8::MaybeLocal maybe_x =
                target->Get(isolate->GetCurrentContext(), key);
            if (maybe_x.IsEmpty()) {
              std::cerr << "Error: there is no x property in saved_object\n";
              break;
            }

            v8::Local<v8::Value> x = maybe_x.ToLocalChecked();
            if (!x->IsInt32()) {
              std::cerr << "Error: x is not an int32 value\n";
              break;
            }
            int32_t x_value =
                x->Int32Value(isolate->GetCurrentContext()).ToChecked();
            std::cout << "print x value: " << x_value << "\n";
          }
          {
            std::lock_guard<std::mutex> guard(finish_cnt_lock);
            finish_cnt++;
          }
        },
        isolate, interval_time, print_times);
    t1.detach();
  }
}

// help function implementation
void DoAdditionLoop(const v8::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() < 1 || info[0]->IsNull() || !info[0]->IsObject()) {
    std::cout << "first paramter is invalid\n";
    return;
  }

  v8::Isolate* isolate = info.GetIsolate();
  v8::Isolate::Scope isolate_scope(isolate);

  v8::HandleScope handle_scope(isolate);

  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  v8::Local<v8::String> key_str = v8::String::NewFromUtf8Literal(isolate, "x");
  v8::Local<v8::Object> pass_in_obj =
      info[0]->ToObject(context).ToLocalChecked();
  v8::MaybeLocal<v8::Value> maybe_x_value = pass_in_obj->Get(context, key_str);
  if (maybe_x_value.IsEmpty()) {
    std::cerr << "Cannot find 'x' property of pass in object.";
    return;
  }
  v8::Local<v8::Value> x_value = maybe_x_value.ToLocalChecked();
  // std::cout << "x value is " << x_value->Int32Value(context).ToChecked()
  //           << "\n";
  save_obj.Reset(isolate, pass_in_obj);

  std::thread t1(
      [](v8::Isolate* isolate) {
        while (true) {
          std::this_thread::sleep_for(std::chrono::milliseconds(200));
          v8::Locker locker(isolate);
          v8::Isolate::Scope isolate_scope(isolate);
          v8::HandleScope handle_scope(isolate);
          v8::Local<v8::Context> context = v8::Context::New(isolate);
          v8::Context::Scope context_scope(context);

          if (context.IsEmpty()) {
            std::cout << "Error! Context is empty\n";
            break;
          }

          v8::Local<v8::String> key =
              v8::String::NewFromUtf8(isolate, "x").ToLocalChecked();

          v8::Local<v8::Object> target =
              v8::Local<v8::Object>::New(isolate, save_obj);

          v8::MaybeLocal<v8::Value> maybe_x_value = target->Get(context, key);
          if (maybe_x_value.IsEmpty()) {
            std::cerr << "cannot find x property!\n";
            break;
          }

          int32_t current =
              maybe_x_value.ToLocalChecked()->Int32Value(context).ToChecked();
          std::cout << "[Print in cpp] current value is " << current << "\n";

          if (current > 5) {
            break;
          }
          
          if (!target->Set(context, key, v8::Int32::New(isolate, current + 1))
                   .ToChecked()) {
            std::cerr << "[Print in cpp] Set x value failed\n";
            break;
          }
          std::cout << "[Print in cpp] set x value to " << current + 1 << "\n";
        }

        {
          std::lock_guard<std::mutex> guard(finish_cnt_lock);
          finish_cnt++;
        }
      },
      isolate);
  t1.detach();
}

void logCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
  std::cout << "std::cout in logCallback\n";
}

void member_TOM_name_getter(v8::Local<v8::String> property,
                            const v8::PropertyCallbackInfo<v8::Value>& info) {
  std::cout << "call in member_TOM_name_getter\n";
}

void member_TOM_name_setter(v8::Local<v8::String> property,
                            v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void>& info) {
  std::cout << "call in member_TOM_name_setter\n";
}

void PointXattrGetter(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value>& info) {
  std::cout << "get in PointXattrGetter\n";
  v8::Local<v8::Object> point_js_object = info.Holder();
  v8::Local<v8::External> point_wrapper =
      v8::Local<v8::External>::Cast(point_js_object->GetInternalField(0));

  void* point_cpp_pointer = point_wrapper->Value();
  int x = static_cast<Point*>(point_cpp_pointer)->x_;
  info.GetReturnValue().Set(x);
}

void PointXattrSetter(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void>& info) {
  v8::Local<v8::Number> js_set_value = v8::Local<v8::Number>::Cast(value);

  std::cout << "get in PointXattrSetter\n";
  v8::Local<v8::Object> point_js_obj = info.Holder();
  v8::Local<v8::External> point_cpp_wrapper =
      v8::Local<v8::External>::Cast(point_js_obj->GetInternalField(0));
  void* point_cpp_pointer = point_cpp_wrapper->Value();
  static_cast<Point*>(point_cpp_pointer)->x_ =
      value->Int32Value(info.GetIsolate()->GetCurrentContext()).ToChecked();
}

void PointYattrGetter(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value>& info) {
  std::cout << "get in PointYattrGetter\n";
  // TODO: Do the same thing as PointXattrGetter();
}

void PointYattrSetter(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void>& info) {
  std::cout << "get in PointYattrSetter\n";
  // TODO: Do the same thing as PointXattrSetter();
}

void PointNamedAttrGetter(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value>& info) {
  std::cout << "get in point named attr GETTER\n";
}

void PointNamedAttrSetter(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<v8::Value>& info) {
  std::cout << "get in point named attr SETTER\n";
}

v8::Local<v8::Array> Create3DCoordinate(int x, int y, int z) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  if (isolate == nullptr) {
    std::cerr << "get isolate failed, isolate is nullpter!\n";
    v8::Local<v8::Array> array;
    return array;
  }

  v8::EscapableHandleScope escape_handle_scope(isolate);

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Array> array_obj = v8::Array::New(isolate, 3);

  if (!array_obj->Set(context, 0, v8::Uint32::New(isolate, x)).ToChecked()) {
    std::cout << "Set new array object error, inmdex 0\n";
  }
  if (!array_obj->Set(context, 1, v8::Uint32::New(isolate, y)).ToChecked()) {
    std::cout << "Set new array object error, inmdex 1\n";
  }
  if (!array_obj->Set(context, 2, v8::Uint32::New(isolate, z)).ToChecked()) {
    std::cout << "Set new array object error, inmdex 2\n";
  }

  return escape_handle_scope.Escape(array_obj);
}
