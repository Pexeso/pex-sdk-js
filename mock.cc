// Copyright 2023 Pexeso Inc. All rights reserved.

#include "fingerprint.h"

#include <pex/sdk/lock.h>
#include <pex/sdk/mockserver.h>

#include "baseworker.h"
#include "context.h"
#include "defer.h"
#include "pexsearch.h"
#include "privatesearch.h"

class MockWorker final : public BaseWorker {
 public:
  MockWorker(Napi::Promise::Deferred& deferred, Pex_Client* client)
      : BaseWorker(deferred), client_(client) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_;
};

void MockWorker::Execute() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  Pex_Mockserver_InitClient(client_, nullptr, status);
  if (!status) {
    return Fail(status);
  }
}

Napi::Value MockWorker::Resolve() {
  return Env().Undefined();
}

// ----------------------------------------------------------------------------

Napi::Value Mock(const Napi::CallbackInfo& info) {
  if (info.Length() != 1 || !info[0].IsObject()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto arg = info[0].As<Napi::Object>();
  auto ctx = info.Env().GetInstanceData<Context>();

  Pex_Client* client = nullptr;

  if (arg.InstanceOf(ctx->pexsearch.Value())) {
    auto search = Napi::ObjectWrap<PexSearch>::Unwrap(arg);
    client = search->client();
  } else if (arg.InstanceOf(ctx->privatesearch.Value())) {
    auto search = Napi::ObjectWrap<PrivateSearch>::Unwrap(arg);
    client = search->client();
  } else {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new MockWorker(d, client);
  w->Queue();
  return d.Promise();
}
