// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _SEARCHOWORKER_H_
#define _SEARCHOWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>
#include <pex/sdk/search.h>

#include "baseworker.h"
#include "fingerprint.h"

class SearchWorker final : public BaseWorker {
 public:
  SearchWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, Fingerprint* ft)
      : BaseWorker(deferred), client_(client), ft_(ft) {}
  virtual ~SearchWorker() override;

  void ExecuteStartSearch();
  void ExecuteCheckSearch();
  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_;
  Fingerprint* ft_;
  std::vector<std::string> lookup_ids_;
  Pex_CheckSearchResult* result_ = nullptr;
};

#endif  // _SEARCHOWORKER_H_
