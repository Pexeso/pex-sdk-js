// Copyright 2023 Pexeso Inc. All rights reserved.

#include "fingerprint.h"

#include <pex/ae/sdk/lock.h>
#include <pex/ae/sdk/mockserver.h>

#include "baseworker.h"
#include "context.h"
#include "defer.h"
#include "pexsearch.h"

class MockWorker final : public BaseWorker {
 public:
  MockWorker(Napi::Promise::Deferred& deferred, AE_Client* client)
      : BaseWorker(deferred), client_(client) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  AE_Client* client_;
};

void MockWorker::Execute() {
  Defer defer;

  AE_Lock();
  defer.Add(AE_Unlock);

  auto status = AE_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(AE_Status_Delete, &status));

  AE_Mockserver_InitClient(client_, nullptr, status);
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

  if (!arg.InstanceOf(ctx->pexsearch.Value())) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto search = Napi::ObjectWrap<PexSearch>::Unwrap(arg);
  auto client = search->client();

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new MockWorker(d, client);
  w->Queue();
  return d.Promise();
}
