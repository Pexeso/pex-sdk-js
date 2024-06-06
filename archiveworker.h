// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _ARCHIVEWORKER_H_
#define _ARCHIVEWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>
#include <pex/sdk/search.h>

#include "baseworker.h"
#include "fingerprint.h"

class ArchiveWorker final : public BaseWorker {
 public:
  ArchiveWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, std::string id, int ft_types)
      : BaseWorker(deferred), client_(client), id_(std::move(id)), ft_types_(ft_types) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_;
  std::string id_;
  int ft_types_;
};

#endif  // _ARCHIVEWORKER_H_
