// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _FINGERPRINTWORKER_H_
#define _FINGERPRINTWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/client.h>

#include "baseworker.h"

class FingerprintWorker final : public BaseWorker {
 public:
  FingerprintWorker(Napi::Promise::Deferred& deferred, std::string input, bool is_file)
      : BaseWorker(deferred), input_(std::move(input)), is_file_(is_file) {}
  virtual ~FingerprintWorker() override {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  std::string input_;
  bool is_file_;
  std::string output_;
};

#endif  // _FINGERPRINTWORKER_H_
