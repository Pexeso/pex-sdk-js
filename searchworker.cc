// Copyright 2023 Pexeso Inc. All rights reserved.

#include "searchworker.h"

#include "defer.h"

void SearchWorker::Execute() {}

Napi::Value SearchWorker::Resolve() {
  return Env().Undefined();
}
