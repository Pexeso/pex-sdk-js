
// Copyright 2024 Pexeso Inc. All rights reserved.

#include "lister.h"

#include "context.h"

Napi::Value Lister::New(Napi::Env env, Pex_Client* client, std::string end_cursor, int limit) {
  auto ctx = env.GetInstanceData<Context>();
  auto lister = ctx->lister.New({});
  auto raw = Napi::ObjectWrap<Lister>::Unwrap(lister);

  raw->client_ = client;
  raw->end_cursor_ = std::move(end_cursor);
  raw->limit_ = limit;

  return lister;
}

Napi::Object Lister::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Lister", {InstanceMethod("list", &Lister::List)});

  auto ctx = env.GetInstanceData<Context>();
  ctx->lister = Napi::Persistent(func);

  exports.Set("Lister", func);
  return exports;
}

Napi::Value Lister::List(const Napi::CallbackInfo& info) {
  // TODO: implement
  return info.Env().Undefined();
}
