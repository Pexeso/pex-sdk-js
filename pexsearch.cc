// Copyright 2023 Pexeso Inc. All rights reserved.

#include "pexsearch.h"

#include <iostream>

#include <pex/ae/sdk/init.h>
#include <pex/ae/sdk/lock.h>

#include "connectworker.h"
#include "fingerprintworker.h"
#include "fingerprint.h"
#include "searchworker.h"
#include "context.h"

Napi::Object PexSearch::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env, "PexSearch",
                  {
                      InstanceMethod("connect", &PexSearch::Connect),
                      InstanceMethod("startSearch", &PexSearch::StartSearch),
                      InstanceMethod("fingerprintFile", &PexSearch::FingerprintFile),
                      InstanceMethod("fingerprintBuffer", &PexSearch::FingerprintBuffer),
                  });

  auto ctx = env.GetInstanceData<Context>();
  ctx->pexsearch = Napi::Persistent(func);

  exports.Set("PexSearch", func);
  return exports;
}

PexSearch::PexSearch(const Napi::CallbackInfo& info) : Napi::ObjectWrap<PexSearch>(info) {
  if (info.Length() != 2 || !info[0].IsString() || !info[1].IsString()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return;
  }

  client_id_ = info[0].As<Napi::String>();
  client_secret_ = info[1].As<Napi::String>();
}

PexSearch::~PexSearch() {
  if (!client_) {
    return;
  }

  AE_Lock();
  AE_Client_Delete(&client_);
  AE_Unlock();

  AE_Cleanup();
}

Napi::Value PexSearch::Connect(const Napi::CallbackInfo& info) {
  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new ConnectWorker(d, &client_, client_id_, client_secret_);
  w->Queue();
  return d.Promise();
}

Napi::Value PexSearch::StartSearch(const Napi::CallbackInfo& info) {
  if (info.Length() != 1 || !info[0].IsObject()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto arg = info[0].As<Napi::Object>();
  auto ctx = info.Env().GetInstanceData<Context>();

  if (!arg.InstanceOf(ctx->fingerprint.Value())) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto ft = Napi::ObjectWrap<Fingerprint>::Unwrap(arg);

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new SearchWorker(d, client_, ft);
  w->Queue();
  return d.Promise();
}

Napi::Value PexSearch::FingerprintFile(const Napi::CallbackInfo& info) {
  if (info.Length() != 1 || !info[0].IsString()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto path = info[0].As<Napi::String>();

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new FingerprintWorker(d, path, true);
  w->Queue();
  return d.Promise();
}

Napi::Value PexSearch::FingerprintBuffer(const Napi::CallbackInfo& info) {
  Napi::Error::New(info.Env(), "Not implemented").ThrowAsJavaScriptException();
  return info.Env().Undefined();
}
