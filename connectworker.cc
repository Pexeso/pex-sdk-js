// Copyright 2023 Pexeso Inc. All rights reserved.

#include "connectworker.h"

#include <pex/ae/sdk/init.h>
#include <pex/ae/sdk/lock.h>
#include <pex/ae/sdk/mockserver.h>

#include "defer.h"

ConnectWorker::~ConnectWorker() {
  if (IsRejected()) {
    AE_Client_Delete(client_p_);
  }
}

void ConnectWorker::Execute() {
  Defer defer;
  int status_code;
  char status_message[100];

  AE_Init(client_id_.c_str(), client_secret_.c_str(), &status_code, status_message,
          sizeof(status_message));
  if (status_code != 0) {
    return Reject(1, "failed to init the library");  // TODO: handle error
  }

  AE_Lock();
  defer.Add(AE_Unlock);

  AE_Status* status = AE_Status_New();
  if (!status) {
    return Reject(1, "out of memory");  // TODO: handle error
  }
  defer.Add(std::bind(AE_Status_Delete, &status));

  *client_p_ = AE_Client_New();
  if (!*client_p_) {
    return Reject(1, "out of memory");  // TODO: handle error
  }

  AE_Client_InitType(*client_p_, AE_PEX_SEARCH, client_id_.c_str(), client_secret_.c_str(), status);
  if (!AE_Status_OK(status)) {
    return Reject(1, "failed to init the client");  // TODO: handle error
  }

  // TODO: remove this after I'm done testing
  AE_Mockserver_InitClient(*client_p_, nullptr, status);
  if (!status) {
    return Reject(1, "failed to mock the client");
  }
}

Napi::Value ConnectWorker::Resolve() {
  return Env().Undefined();
}
