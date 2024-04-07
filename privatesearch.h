// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _PRIVATESEARCH_H_
#define _PRIVATESEARCH_H_

#include <napi.h>

#include <pex/sdk/client.h>

#include "fingerprinter.h"

class PrivateSearch final : public Napi::ObjectWrap<PrivateSearch>, public Fingerprinter {
  friend class Fingerprinter;

  static Napi::FunctionReference constructor;

 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  PrivateSearch(const Napi::CallbackInfo& info);
  ~PrivateSearch();

  Pex_Client* client() const { return client_; }

 private:
  Napi::Value Connect(const Napi::CallbackInfo& info);
  Napi::Value StartSearch(const Napi::CallbackInfo& info);
  Napi::Value Ingest(const Napi::CallbackInfo& info);

  std::string client_id_;
  std::string client_secret_;

  Pex_Client* client_ = nullptr;
};

#endif  // _PRIVATESEARCH_H_
