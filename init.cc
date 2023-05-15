// Copyright 2023 Pexeso Inc. All rights reserved.

#include <napi.h>

#include "context.h"
#include "fingerprint.h"
#include "pexsearch.h"
#include "error.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  env.SetInstanceData(new Context);

  exports = Fingerprint::Init(env, exports);
  exports = PexSearch::Init(env, exports);
  exports = Error::Init(env, exports);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
