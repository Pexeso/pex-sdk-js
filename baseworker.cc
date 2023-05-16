// Copyright 2023 Pexeso Inc. All rights reserved.

#include "baseworker.h"
#include "error.h"

void BaseWorker::Fail(const AE_Status* status) {
  Fail(AE_Status_GetCode(status), AE_Status_GetMessage(status));
}

void BaseWorker::Fail(int code, const std::string& message) {
  status_code_ = code;
  status_message_ = message;
}

bool BaseWorker::Failed() {
  return status_code_ != 0;
}

void BaseWorker::OnOK() {
  auto val = Resolve();
  if (Failed()) {
    Reject();
    return;
  }
  deferred_.Resolve(val);
}

void BaseWorker::OnError(const Napi::Error& error) {
  if (status_code_ == 0) {
    status_code_ = ErrorCode::INTERNAL_ERROR;
  }
  status_message_ = error.Message();
  Reject();
}

void BaseWorker::Reject() {
  auto err = Error::New(Env(), status_code_, status_message_);
  deferred_.Reject(err);
}

void BaseWorker::OOM() {
  Fail(ErrorCode::OUT_OF_MEMORY, "out of memory");
}
