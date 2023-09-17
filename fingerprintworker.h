// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _FINGERPRINTWORKER_H_
#define _FINGERPRINTWORKER_H_

#include <napi.h>

#include <pex/sdk/fingerprint.h>

#include "baseworker.h"

class FingerprintWorker final : public BaseWorker {
 public:
  FingerprintWorker(Napi::Promise::Deferred& deferred, std::string_view input, bool is_file,
                    int type)
      : BaseWorker(deferred), input_(std::move(input)), is_file_(is_file), type_(type) {}
  FingerprintWorker(Napi::Promise::Deferred& deferred, std::string input_buffer, bool is_file,
                    int type)
      : BaseWorker(deferred),
        input_buffer_(std::move(input_buffer)),
        input_(input_buffer_),
        is_file_(is_file),
        type_(type) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  std::string input_buffer_;
  std::string_view input_;
  bool is_file_;
  int type_;
  std::string output_;
};

#endif  // _FINGERPRINTWORKER_H_
