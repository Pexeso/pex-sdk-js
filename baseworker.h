// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _BASEWORKER_H_
#define _BASEWORKER_H_

#include <napi.h>

#include <pex/sdk/status.h>

class BaseWorker : public Napi::AsyncWorker {
 protected:
  BaseWorker(Napi::Promise::Deferred& deferred)
      : Napi::AsyncWorker(deferred.Env()), deferred_(deferred) {}
  virtual ~BaseWorker() override {}

  virtual Napi::Value Resolve() = 0;

  void Fail(const Pex_Status* status);
  void Fail(int code, const std::string& msg, bool is_retryable = false);
  bool Failed();

  void OOM();

 private:
  void OnOK() final override;
  void OnError(const Napi::Error& error) final override;
  void Reject();

  Napi::Promise::Deferred deferred_;
  int status_code_ = 0;
  std::string status_message_;
  bool is_retryable_ = false;
};

#endif  // _BASEWORKER_H_
