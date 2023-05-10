// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _CONNECTOWORKER_H_
#define _CONNECTOWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/client.h>

class ConnectWorker : public Napi::AsyncWorker {
 public:
  ConnectWorker(Napi::Promise::Deferred& deferred, AE_Client** client_p, std::string client_id,
                std::string client_secret)
      : Napi::AsyncWorker(deferred.Env()),
        deferred_(deferred),
        client_p_(client_p),
        client_id_(std::move(client_id)),
        client_secret_(std::move(client_secret)) {}

  void Execute() override;
  void OnOK() override;
  void OnError(const Napi::Error& error) override;

 private:
  Napi::Promise::Deferred deferred_;
  AE_Client** client_p_;
  std::string client_id_;
  std::string client_secret_;
};

#endif  // _CONNECTOWORKER_H_
