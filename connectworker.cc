// Copyright 2023 Pexeso Inc. All rights reserved.

#include "connectworker.h"

#include <pex/ae/sdk/init.h>
#include <pex/ae/sdk/lock.h>

#include "defer.h"
#include "error.h"

ConnectWorker::~ConnectWorker() {
  if (Failed()) {
    AE_Client_Delete(client_p_);
  }
}

void ConnectWorker::Execute() {
  Defer defer;
  int status_code;
  char status_message[100];

  AE_Init(client_id_.data(), client_secret_.data(), &status_code, status_message,
          sizeof(status_message));
  if (status_code != ErrorCode::OK) {
    return Fail(status_code, "failed to init the library");
  }

  AE_Lock();
  defer.Add(AE_Unlock);

  auto status = AE_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(AE_Status_Delete, &status));

  *client_p_ = AE_Client_New();
  if (!*client_p_) {
    return OOM();
  }

  AE_Client_InitType(*client_p_, client_type_, client_id_.data(), client_secret_.data(), status);
  if (!AE_Status_OK(status)) {
    return Fail(status);
  }
}

Napi::Value ConnectWorker::Resolve() {
  return Env().Undefined();
}
