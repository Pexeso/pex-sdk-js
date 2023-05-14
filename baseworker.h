// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _BASEWORKER_H_
#define _BASEWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/status.h>

class BaseWorker : public Napi::AsyncWorker {
 protected:
  BaseWorker(Napi::Promise::Deferred& deferred)
      : Napi::AsyncWorker(deferred.Env()), deferred_(deferred) {}
  virtual ~BaseWorker() override {}

  virtual Napi::Value Resolve() = 0;

  void Reject(const AE_Status* status);
  void Reject(int code, const std::string& msg);
  bool IsRejected();

 private:
  void OnOK() final override;
  void OnError(const Napi::Error& error) final override;

  Napi::Promise::Deferred deferred_;
  int status_code_ = 0;
};

#endif  // _BASEWORKER_H_
