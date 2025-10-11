// Copyright 2025 Pexeso Inc. All rights reserved.

#ifndef _GETWORKER_H_
#define _GETWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>
#include <pex/sdk/search.h>

#include "baseworker.h"

class GetWorker final : public BaseWorker {
 public:
  GetWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, std::string id)
      : BaseWorker(deferred), client_(client), id_(std::move(id)) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_;
  std::string id_;

  std::string result_json_;
};

#endif  // _GETWORKER_H_
