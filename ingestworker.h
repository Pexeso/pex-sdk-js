// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _INGESTWORKER_H_
#define _INGESTWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>
#include <pex/sdk/search.h>

#include "baseworker.h"
#include "fingerprint.h"

class IngestWorker final : public BaseWorker {
 public:
  IngestWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, std::string id,
               Fingerprint* ft)
      : BaseWorker(deferred), client_(client), id_(std::move(id)), ft_(ft) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_;
  std::string id_;
  Fingerprint* ft_;
};

#endif  // _INGESTWORKER_H_
