// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _LISTWORKER_H_
#define _LISTWORKER_H_

#include <napi.h>

#include <pex/sdk/client.h>
#include <pex/sdk/search.h>

#include "baseworker.h"

class ListWorker final : public BaseWorker {
 public:
  ListWorker(Napi::Promise::Deferred& deferred, Pex_Client* client, std::string* end_cursor,
             int* limit)
      : BaseWorker(deferred), client_(client), end_cursor_(end_cursor), limit_(limit) {}

  void Execute() override;
  Napi::Value Resolve() override;

 private:
  Pex_Client* client_ = nullptr;
  std::string* end_cursor_ = nullptr;
  int* limit_ = nullptr;

  std::string result_json_;
};

#endif  // _LISTWORKER_H_
