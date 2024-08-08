// Copyright 2023 Pexeso Inc. All rights reserved.

#include "privatesearch.h"

#include <pex/sdk/init.h>
#include <pex/sdk/lock.h>

#include "connectworker.h"
#include "searchworker.h"
#include "ingestworker.h"
#include "archiveworker.h"
#include "context.h"

Napi::Object PrivateSearch::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env, "PrivateSearchClient",
                  {
                      InstanceMethod("connect", &PrivateSearch::Connect),
                      InstanceMethod("startSearch", &PrivateSearch::StartSearch),
                      InstanceMethod("ingest", &PrivateSearch::Ingest),
                      InstanceMethod("archive", &PrivateSearch::Archive),
                      InstanceMethod("fingerprintFile", &PrivateSearch::FingerprintFile),
                      InstanceMethod("fingerprintBuffer", &PrivateSearch::FingerprintBuffer),
                  });

  auto ctx = env.GetInstanceData<Context>();
  ctx->privatesearch = Napi::Persistent(func);

  exports.Set("PrivateSearchClient", func);
  return exports;
}

PrivateSearch::PrivateSearch(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PrivateSearch>(info) {
  if (info.Length() != 2 || !info[0].IsString() || !info[1].IsString()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return;
  }

  client_id_ = info[0].As<Napi::String>();
  client_secret_ = info[1].As<Napi::String>();
}

PrivateSearch::~PrivateSearch() {
  if (!client_) {
    return;
  }

  Pex_Lock();
  Pex_Client_Delete(&client_);
  Pex_Unlock();

  Pex_Cleanup();
}

Napi::Value PrivateSearch::Connect(const Napi::CallbackInfo& info) {
  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new ConnectWorker(d, Pex_PRIVATE_SEARCH, &client_, client_id_, client_secret_);
  w->Queue();
  return d.Promise();
}

Napi::Value PrivateSearch::StartSearch(const Napi::CallbackInfo& info) {
  if (info.Length() != 1 || !info[0].IsObject()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto arg = info[0].As<Napi::Object>();
  auto ctx = info.Env().GetInstanceData<Context>();

  Napi::Array props = arg.GetPropertyNames();
  for (uint32_t i = 0; i < props.Length(); i++) {
    std::string prop = props.Get(i).ToString();
    if (prop != "fingerprint") {
      Napi::Error::New(info.Env(), "Unrecognized option").ThrowAsJavaScriptException();
      return info.Env().Undefined();
    }
  }

  auto ft_arg = arg.Get("fingerprint");
  if (!ft_arg.IsObject() || !ft_arg.ToObject().InstanceOf(ctx->fingerprint.Value())) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }
  auto ft = Napi::ObjectWrap<Fingerprint>::Unwrap(ft_arg.ToObject());

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new SearchWorker(d, client_, ft, Pex_SearchType_Default);
  w->Queue();
  return d.Promise();
}

Napi::Value PrivateSearch::Ingest(const Napi::CallbackInfo& info) {
  if (info.Length() != 2 || !info[0].IsString() || !info[1].IsObject()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto ctx = info.Env().GetInstanceData<Context>();

  auto ft_arg = info[1].ToObject();
  if (!ft_arg.InstanceOf(ctx->fingerprint.Value())) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto id = info[0].ToString();
  auto ft = Napi::ObjectWrap<Fingerprint>::Unwrap(ft_arg.ToObject());

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new IngestWorker(d, client_, id, ft);
  w->Queue();
  return d.Promise();
}

Napi::Value PrivateSearch::Archive(const Napi::CallbackInfo& info) {
  if (info.Length() == 0 || !info[0].IsString()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto id = info[0].ToString();

  auto ft_types = GetFingerprintTypes(info);
  if (info.Env().IsExceptionPending()) {
    return info.Env().Undefined();
  }

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new ArchiveWorker(d, client_, id, ft_types);
  w->Queue();
  return d.Promise();
}
