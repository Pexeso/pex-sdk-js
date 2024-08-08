// Copyright 2024 Pexeso Inc. All rights reserved.

#ifndef _LISTER_H_
#define _LISTER_H_

#include <napi.h>

#include <pex/sdk/client.h>

class Lister final : public Napi::ObjectWrap<Lister> {
 public:
  static Napi::Value New(Napi::Env env, Pex_Client* client, std::string end_cursor, int limit);
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  Lister(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Lister>(info) {}

 private:
  Napi::Value List(const Napi::CallbackInfo& info);
  Napi::Value EndCursor(const Napi::CallbackInfo& info);
  Napi::Value HasNextPage(const Napi::CallbackInfo& info);

 private:
  Pex_Client* client_ = nullptr;
  std::string end_cursor_;
  int limit_ = 0;
  bool has_next_page_ = true;
};

#endif  // _LISTER_H_
