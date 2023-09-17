// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _PEXSEARCH_H_
#define _PEXSEARCH_H_

#include <napi.h>

#include <pex/sdk/client.h>

class PexSearch final : public Napi::ObjectWrap<PexSearch> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  PexSearch(const Napi::CallbackInfo& info);
  ~PexSearch();

  Pex_Client* client() const { return client_; }

 private:
  Napi::Value Connect(const Napi::CallbackInfo& info);
  Napi::Value StartSearch(const Napi::CallbackInfo& info);
  Napi::Value FingerprintFile(const Napi::CallbackInfo& info);
  Napi::Value FingerprintBuffer(const Napi::CallbackInfo& info);

  std::string client_id_;
  std::string client_secret_;

  Pex_Client* client_ = nullptr;
};

#endif  // _PEXSEARCH_H_
