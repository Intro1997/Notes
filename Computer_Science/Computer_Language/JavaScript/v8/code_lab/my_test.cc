// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "my_test.hpp"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include <functional>
// #include <iostream>

// #include "include/libplatform/libplatform.h"
// #include "include/v8-context.h"
// #include "include/v8-initialization.h"
// #include "include/v8-isolate.h"
// #include "include/v8-local-handle.h"
// #include "include/v8-primitive.h"
// #include "include/v8-script.h"
// #include "include/v8-template.h"
// #include "src/debug/interface-types.h"

class ConsoleLog : public v8::debug::ConsoleDelegate {
 public:
  void Log(const v8::debug::ConsoleCallArguments& args,
           const v8::debug::ConsoleContext& context) override {}
};

void IsolateTest(int argc, char* argv[]) {
  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[1]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();

  v8::Isolate* isolate = v8::Isolate::New(create_params);
  TestLocker(isolate);
  // GlobalPropertyTest(isolate);
  // ObjectAccessorTest(isolate);
  // ConnectCppObjectWithJsObject(isolate);
  // TryInterceptor(isolate);
  // GetArrayFromFuncReturn(isolate);
  // TryExceptionInJs(isolate);
  // TestInherit(isolate);
  // IsolateScopeTest(isolate);

  // ContextScopeTest(isolate);
  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::DisposePlatform();
  delete create_params.array_buffer_allocator;
}

void V8Test(int argc, char* argv[]) { TestArrayBufferAllocator(argc, argv); }

int main(int argc, char* argv[]) {
  IsolateTest(argc, argv);

  // V8Test(argc, argv);
  return 0;
}
