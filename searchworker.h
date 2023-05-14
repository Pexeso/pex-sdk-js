// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _SEARCHOWORKER_H_
#define _SEARCHOWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/client.h>
#include <pex/ae/sdk/search.h>

#include "baseworker.h"
#include "fingerprint.h"

class SearchWorker final : public BaseWorker {
 public:
  SearchWorker(Napi::Promise::Deferred& deferred, AE_Client* client, Fingerprint* ft)
      : BaseWorker(deferred), client_(client), ft_(ft) {}
  virtual ~SearchWorker() override;

  void ExecuteStartSearch();
  void ExecuteCheckSearch();
  void Execute() override;
  Napi::Value Resolve() override;

 private:
  AE_Client* client_;
  Fingerprint* ft_;
  std::string lookup_id_;
  AE_CheckSearchResult* result_ = nullptr;
};

#endif  // _SEARCHOWORKER_H_
