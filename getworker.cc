// Copyright 2025 Pexeso Inc. All rights reserved.

#include "getworker.h"

#include <pex/sdk/lock.h>
#include <pex/sdk/ingestion.h>

#include "defer.h"

void GetWorker::Execute() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  auto buffer = Pex_Buffer_New();
  if (!buffer) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Buffer_Delete, &buffer));

  Pex_Get(client_, id_.c_str(), buffer, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }

  auto data = static_cast<const char*>(Pex_Buffer_GetData(buffer));
  auto size = Pex_Buffer_GetSize(buffer);
  result_json_.assign(data, size);
}

Napi::Value GetWorker::Resolve() {
  auto result = Napi::String::New(Env(), result_json_);

  auto json = Env().Global().Get("JSON").As<Napi::Object>();
  auto parse = json.Get("parse").As<Napi::Function>();

  return parse.Call(json, {result}).ToObject();
}
