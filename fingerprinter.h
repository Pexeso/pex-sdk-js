// Copyright 2024 Pexeso Inc. All rights reserved.

#ifndef _FINGERPRINTER_H_
#define _FINGERPRINTER_H_

#include <napi.h>

#include <pex/sdk/client.h>

class Fingerprinter {
 public:
  virtual ~Fingerprinter() {}

  Napi::Value FingerprintFile(const Napi::CallbackInfo& info);
  Napi::Value FingerprintBuffer(const Napi::CallbackInfo& info);

 protected:
  Pex_Client* client_ = nullptr;
};

#endif  // _PEXSEARCH_H_
