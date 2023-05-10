// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _SEARCHOWORKER_H_
#define _SEARCHOWORKER_H_

#include <napi.h>

#include <pex/ae/sdk/client.h>

#include "baseworker.h"

class SearchWorker final : public BaseWorker {
 public:
  SearchWorker(Napi::Promise::Deferred& deferred) : BaseWorker(deferred) {}

  void Execute() final override;
  Napi::Value Resolve() final;
};

#endif  // _SEARCHOWORKER_H_
