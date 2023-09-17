// Copyright 2023 Pexeso Inc. All rights reserved.

#include "fingerprintworker.h"

#include <pex/sdk/buffer.h>
#include <pex/sdk/fingerprint.h>
#include <pex/sdk/lock.h>

#include "defer.h"
#include "fingerprint.h"
#include "error.h"

void FingerprintWorker::Execute() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  Pex_Status* status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  Pex_Buffer* out_buf = Pex_Buffer_New();
  if (!out_buf) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Buffer_Delete, &out_buf));

  if (is_file_) {
    Pex_Fingerprint_File_For_Types(input_.data(), out_buf, status, type_);
  } else {
    Pex_Buffer* in_buf = Pex_Buffer_New();
    if (!in_buf) {
      return OOM();
    }
    defer.Add(std::bind(Pex_Buffer_Delete, &in_buf));

    Pex_Buffer_Set(in_buf, input_.data(), input_.size());

    Pex_Fingerprint_Buffer_For_Types(in_buf, out_buf, status, type_);
  }

  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }

  auto data = static_cast<const char*>(Pex_Buffer_GetData(out_buf));
  auto size = Pex_Buffer_GetSize(out_buf);
  output_.assign(data, size);
}

Napi::Value FingerprintWorker::Resolve() {
  return Fingerprint::New(Env(), std::move(output_));
}
