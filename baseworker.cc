// Copyright 2023 Pexeso Inc. All rights reserved.

#include "baseworker.h"

void BaseWorker::Reject(const AE_Status* status) {
  Reject(AE_Status_GetCode(status), AE_Status_GetMessage(status));
}

void BaseWorker::Reject(int code, const std::string& message) {
  status_code_ = code;
  SetError(message);
}

bool BaseWorker::IsRejected() {
  return status_code_ != 0;
}

void BaseWorker::OnOK() {
  auto val = Resolve();
  if (IsRejected()) {
    deferred_.Reject(Napi::String::New(Env(), "BAD SHIT"));  // TODO
    return;
  }
  deferred_.Resolve(val);
}

void BaseWorker::OnError(const Napi::Error& error) {
  if (status_code_ == 0) {  // TODO: change to constants when possible
    status_code_ = 7;
  }
  deferred_.Reject(error.Value());
}
