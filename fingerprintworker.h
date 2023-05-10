// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _FINGERPRINTWORKER_H_
#define _FINGERPRINTWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/client.h>

class FingerprintWorker : public Napi::AsyncWorker {
 public:
  FingerprintWorker(Napi::Promise::Deferred& deferred, std::string input, bool is_file)
      : Napi::AsyncWorker(deferred.Env()),
        deferred_(deferred),
        input_(std::move(input)),
        is_file_(is_file) {}

  void Execute() override;
  void OnOK() override;
  void OnError(const Napi::Error& error) override;

 private:
  Napi::Promise::Deferred deferred_;
  std::string input_;
  bool is_file_;
  std::string output_;
};

#endif  // _FINGERPRINTWORKER_H_
