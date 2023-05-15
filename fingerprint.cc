// Copyright 2023 Pexeso Inc. All rights reserved.

#include "fingerprint.h"

#include "context.h"

Napi::Value Fingerprint::New(Napi::Env env, std::string bytes) {
  auto ctx = env.GetInstanceData<Context>();
  auto ft = ctx->fingerprint.New({});
  auto raw = Napi::ObjectWrap<Fingerprint>::Unwrap(ft);
  raw->set_bytes(std::move(bytes));
  return ft;
}

Napi::Object Fingerprint::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env, "Fingerprint", {InstanceMethod("dump", &Fingerprint::Dump)});

  auto ctx = env.GetInstanceData<Context>();
  ctx->fingerprint = Napi::Persistent(func);

  exports.Set("Fingerprint", func);
  return exports;
}

Napi::Value Fingerprint::Dump(const Napi::CallbackInfo& info) {
  return Napi::Buffer<char>::New(info.Env(), bytes_.data(), bytes_.size());
}
