// Copyright 2023 Pexeso Inc. All rights reserved.

#include "baseworker.h"

void BaseWorker::Reject(const AE_Status* status) {
  Reject(AE_Status_GetCode(status), AE_Status_GetMessage(status));
}

void BaseWorker::Reject(int code, const std::string& message) {
  status_code_ = code;
  status_message_ = message;
}

void BaseWorker::OnOK() {
  if (status_code_ != 0) {
    deferred_.Reject(Napi::String::New(Env(), status_message_));
    return;
  }
  deferred_.Resolve(Resolve());
}
