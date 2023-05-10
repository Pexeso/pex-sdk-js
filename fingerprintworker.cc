// Copyright 2023 Pexeso Inc. All rights reserved.

#include "fingerprintworker.h"

#include <pex/ae/sdk/buffer.h>
#include <pex/ae/sdk/fingerprint.h>
#include <pex/ae/sdk/lock.h>

#include "defer.h"
#include "fingerprint.h"

void FingerprintWorker::Execute() {
  Defer defer;

  AE_Lock();
  defer.Add(AE_Unlock);

  AE_Status* status = AE_Status_New();
  if (!status) {
    return SetError("out of memory");  // TODO: handle error
  }
  defer.Add(std::bind(AE_Status_Delete, &status));

  AE_Buffer* out_buf = AE_Buffer_New();
  if (!out_buf) {
    return SetError("out of memory");  // TODO: handle error
  }
  defer.Add(std::bind(AE_Buffer_Delete, &out_buf));

  if (is_file_) {
    AE_Fingerprint_File_For_Types(
        input_.c_str(), out_buf, status,
        AE_Fingerprint_Type_Audio);  // TODO: specify real fingerprint type
  } else {
    AE_Buffer* in_buf = AE_Buffer_New();
    if (!in_buf) {
      return SetError("out of memory");  // TODO: handle error
    }
    defer.Add(std::bind(AE_Buffer_Delete, &in_buf));

    AE_Buffer_Set(in_buf, input_.c_str(), input_.size());

    AE_Fingerprint_Buffer_For_Types(
        in_buf, out_buf, status, AE_Fingerprint_Type_Audio);  // TODO: specify real fingerprint type
  }

  if (!AE_Status_OK(status)) {
    return SetError("failed to fingerprint");
  }

  auto data = static_cast<const char*>(AE_Buffer_GetData(out_buf));
  auto size = AE_Buffer_GetSize(out_buf);
  output_.assign(data, size);
}

void FingerprintWorker::OnOK() {
  deferred_.Resolve(Fingerprint::Make(Env(), std::move(output_)));
}

void FingerprintWorker::OnError(const Napi::Error& error) {
  deferred_.Reject(error.Value());
}
