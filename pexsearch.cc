// Copyright 2023 Pexeso Inc. All rights reserved.

#include "pexsearch.h"

#include <pex/sdk/init.h>
#include <pex/sdk/lock.h>

#include "connectworker.h"
#include "searchworker.h"
#include "context.h"

Napi::Object PexSearch::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env, "PexSearchClient",
                  {
                      InstanceMethod("connect", &PexSearch::Connect),
                      InstanceMethod("startSearch", &PexSearch::StartSearch),
                      InstanceMethod("fingerprintFile", &PexSearch::FingerprintFile),
                      InstanceMethod("fingerprintBuffer", &PexSearch::FingerprintBuffer),
                  });

  auto ctx = env.GetInstanceData<Context>();
  ctx->pexsearch = Napi::Persistent(func);

  exports.Set("PexSearchClient", func);
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

  Pex_Lock();
  Pex_Client_Delete(&client_);
  Pex_Unlock();

  Pex_Cleanup();
}

Napi::Value PexSearch::Connect(const Napi::CallbackInfo& info) {
  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new ConnectWorker(d, Pex_PEX_SEARCH, &client_, client_id_, client_secret_);
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

  Napi::Array props = arg.GetPropertyNames();
  for (uint32_t i = 0; i < props.Length(); i++) {
    std::string prop = props.Get(i).ToString();
    if (prop != "fingerprint" && prop != "type") {
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

  auto search_type = Pex_SearchType_Default;
  auto search_type_arg = arg.Get("type");

  if (search_type_arg.IsString()) {
    std::string str = search_type_arg.ToString();
    if (str == kFindMatches) {
      search_type = Pex_SearchType_FindMatches;
    } else if (str == kIdentifyMusic) {
      search_type = Pex_SearchType_IdentifyMusic;
    } else {
      Napi::Error::New(info.Env(), "Invalid search type").ThrowAsJavaScriptException();
      return info.Env().Undefined();
    }
  }

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new SearchWorker(d, client_, ft, search_type);
  w->Queue();
  return d.Promise();
}
