// Copyright 2023 Pexeso Inc. All rights reserved.

#include "connectworker.h"

#include <pex/ae/sdk/init.h>
#include <pex/ae/sdk/lock.h>

#include "defer.h"

void ConnectWorker::Execute() {
  Defer defer;
  int status_code;
  char status_message[100];

  AE_Init(client_id_.c_str(), client_secret_.c_str(), &status_code, status_message,
          sizeof(status_message));
  if (status_code != 0) {
    return SetError("failed to init the library");  // TODO: handle error
  }

  AE_Lock();
  defer.Add(AE_Unlock);

  AE_Status* status = AE_Status_New();
  if (!status) {
    return SetError("out of memory");  // TODO: handle error
  }
  defer.Add(std::bind(AE_Status_Delete, &status));

  *client_p_ = AE_Client_New();
  if (!*client_p_) {
    return SetError("out of memory");  // TODO: handle error
  }

  AE_Client_InitType(*client_p_, AE_PEX_SEARCH, client_id_.c_str(), client_secret_.c_str(), status);
  if (!AE_Status_OK(status)) {
    AE_Client_Delete(client_p_);
    return SetError("failed to init the client");  // TODO: handle error
  }
}

void ConnectWorker::OnOK() {
  deferred_.Resolve(Env().Undefined());
}

void ConnectWorker::OnError(const Napi::Error& error) {
  deferred_.Reject(error.Value());
}
