// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _CONNECTOWORKER_H_
#define _CONNECTOWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>

#include "baseworker.h"

class ConnectWorker final : public BaseWorker {
 public:
  ConnectWorker(Napi::Promise::Deferred& deferred, Pex_ClientType client_type,
                Pex_Client** client_p, std::string client_id, std::string client_secret)
      : BaseWorker(deferred),
        client_type_(client_type),
        client_p_(client_p),
        client_id_(std::move(client_id)),
        client_secret_(std::move(client_secret)) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_ClientType client_type_;
  Pex_Client** client_p_ = nullptr;
  std::string client_id_;
  std::string client_secret_;
};

#endif  // _CONNECTOWORKER_H_
