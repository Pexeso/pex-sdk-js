// Copyright 2023 Pexeso Inc. All rights reserved.

#include <napi.h>

#include "context.h"
#include "error.h"
#include "fingerprint.h"
#include "pexsearch.h"
#include "privatesearch.h"
#include "lister.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  env.SetInstanceData(new Context);

  exports = Fingerprint::Init(env, exports);
  exports = PexSearch::Init(env, exports);
  exports = PrivateSearch::Init(env, exports);
  exports = Error::Init(env, exports);
  exports = Lister::Init(env, exports);

  exports.Set("AUDIO", kAudio);
  exports.Set("VIDEO", kVideo);
  exports.Set("MELODY", kMelody);
  exports.Set("PHONETIC", kPhonetic);
  exports.Set("CLASSIFICATION", kClassification);

  exports.Set("FIND_MATCHES", kFindMatches);
  exports.Set("IDENTIFY_MUSIC", kIdentifyMusic);

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
