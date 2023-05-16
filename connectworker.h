// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _CONNECTOWORKER_H_
#define _CONNECTOWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/client.h>

#include "baseworker.h"

class ConnectWorker final : public BaseWorker {
 public:
  ConnectWorker(Napi::Promise::Deferred& deferred, AE_Client** client_p, std::string client_id,
                std::string client_secret)
      : BaseWorker(deferred),
        client_p_(client_p),
        client_id_(std::move(client_id)),
        client_secret_(std::move(client_secret)) {}
  ~ConnectWorker();

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  AE_Client** client_p_ = nullptr;
  std::string client_id_;
  std::string client_secret_;
};

#endif  // _CONNECTOWORKER_H_
