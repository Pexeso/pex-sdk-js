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

Fingerprint::Fingerprint(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Fingerprint>(info) {
  if (info.Length() == 0) {
    return;
  }

  if (!info[0].IsBuffer()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return;
  }

  auto arg = info[0].As<Napi::Uint8Array>();
  std::string buf(reinterpret_cast<char*>(arg.Data()), arg.ByteLength());

  set_bytes(std::move(buf));
}

Napi::Value Fingerprint::Dump(const Napi::CallbackInfo& info) {
  return Napi::Buffer<char>::New(info.Env(), bytes_.data(), bytes_.size());
}
