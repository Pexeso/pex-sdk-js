// Copyright 2023 Pexeso Inc. All rights reserved.

#include <napi.h>

#include "context.h"
#include "error.h"
#include "fingerprint.h"
#include "mock.h"
#include "pexsearch.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  env.SetInstanceData(new Context);

  exports = Fingerprint::Init(env, exports);
  exports = PexSearch::Init(env, exports);
  exports = Error::Init(env, exports);

  exports.Set("mock", Napi::Function::New(env, Mock));

  exports.Set("AUDIO", kAudio);
  exports.Set("VIDEO", kVideo);
  exports.Set("MELODY", kMelody);

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
