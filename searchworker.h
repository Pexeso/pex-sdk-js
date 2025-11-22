// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _SEARCHWORKER_H_
#define _SEARCHWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>
#include <pex/sdk/search.h>

#include "baseworker.h"
#include "fingerprint.h"

class SearchWorker final : public BaseWorker {
 public:
  // Regular search with fingerprint
  SearchWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, Fingerprint* ft,
               Pex_SearchType search_type)
      : BaseWorker(deferred), client_(client), ft_(ft), search_type_(search_type) {}

  // ISRC search
  SearchWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, std::string isrc,
               int ft_types, Pex_SearchType search_type)
      : BaseWorker(deferred),
        client_(client),
        isrc_(std::move(isrc)),
        ft_types_(ft_types),
        search_type_(search_type) {}

  virtual ~SearchWorker() override;

  void ExecuteStartSearch();
  void ExecuteCheckSearch();
  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_ = nullptr;
  Fingerprint* ft_ = nullptr;
  std::string isrc_;
  int ft_types_ = 0;
  Pex_SearchType search_type_ = Pex_SearchType_Default;
  std::vector<std::string> lookup_ids_;
  Pex_CheckSearchResult* result_ = nullptr;
};

#endif  // _SEARCHWORKER_H_
