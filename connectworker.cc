// Copyright 2023 Pexeso Inc. All rights reserved.

#include "connectworker.h"

#include <pex/sdk/init.h>
#include <pex/sdk/lock.h>

#include "defer.h"
#include "error.h"

ConnectWorker::~ConnectWorker() {
  if (Failed()) {
    Pex_Client_Delete(client_p_);
  }
}

void ConnectWorker::Execute() {
  Defer defer;
  int status_code;
  char status_message[100];

  Pex_Init(client_id_.data(), client_secret_.data(), &status_code, status_message,
           sizeof(status_message));
  if (status_code != ErrorCode::OK) {
    return Fail(status_code, "failed to init the library");
  }

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  *client_p_ = Pex_Client_New();
  if (!*client_p_) {
    return OOM();
  }

  Pex_Client_Init(*client_p_, client_type_, client_id_.data(), client_secret_.data(), status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }
}

Napi::Value ConnectWorker::Resolve() {
  return Env().Undefined();
}
