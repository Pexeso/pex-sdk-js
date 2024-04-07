// Copyright 2024 Pexeso Inc. All rights reserved.

#ifndef _FINGERPRINTER_H_
#define _FINGERPRINTER_H_

#include <napi.h>

class Fingerprinter {
 public:
  Napi::Value FingerprintFile(const Napi::CallbackInfo& info);
  Napi::Value FingerprintBuffer(const Napi::CallbackInfo& info);
};

#endif  // _PEXSEARCH_H_
