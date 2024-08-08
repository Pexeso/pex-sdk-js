// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _PEXSEARCH_H_
#define _PEXSEARCH_H_

#include <napi.h>

#include "fingerprinter.h"

const auto kFindMatches = "find_matches";
const auto kIdentifyMusic = "identify_music";

class PexSearch final : public Napi::ObjectWrap<PexSearch>, public Fingerprinter {
  static Napi::FunctionReference constructor;

 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  PexSearch(const Napi::CallbackInfo& info);
  ~PexSearch();

  Pex_Client* client() const { return client_; }

 private:
  Napi::Value Connect(const Napi::CallbackInfo& info);
  Napi::Value StartSearch(const Napi::CallbackInfo& info);

  std::string client_id_;
  std::string client_secret_;
};

#endif  // _PEXSEARCH_H_
