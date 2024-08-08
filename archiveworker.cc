// Copyright 2023 Pexeso Inc. All rights reserved.

#include "archiveworker.h"

#include <pex/sdk/lock.h>
#include <pex/sdk/ingestion.h>

#include "defer.h"

void ArchiveWorker::Execute() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  Pex_Archive(client_, id_.c_str(), ft_types_, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }
}

Napi::Value ArchiveWorker::Resolve() {
  return Env().Undefined();
}
