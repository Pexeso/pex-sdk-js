// Copyright 2023 Pexeso Inc. All rights reserved.

#include "ingestworker.h"

#include <pex/sdk/lock.h>
#include <pex/sdk/ingestion.h>

#include "defer.h"

void IngestWorker::Execute() {
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

  Pex_Buffer_Set(buffer, ft_->bytes().data(), ft_->bytes().size());

  Pex_Ingest(client_, id_.c_str(), buffer, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }
}

Napi::Value IngestWorker::Resolve() {
  return Env().Undefined();
}
