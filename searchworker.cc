// Copyright 2023 Pexeso Inc. All rights reserved.

#include "searchworker.h"

#include <pex/sdk/lock.h>

#include "defer.h"

SearchWorker::~SearchWorker() {
  Pex_CheckSearchResult_Delete(&result_);
}

void SearchWorker::Execute() {
  ExecuteStartSearch();
  if (!Failed()) {
    ExecuteCheckSearch();
  }
}

void SearchWorker::ExecuteStartSearch() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  auto request = Pex_StartSearchRequest_New();
  if (!request) {
    return OOM();
  }
  defer.Add(std::bind(Pex_StartSearchRequest_Delete, &request));

  auto result = Pex_StartSearchResult_New();
  if (!result) {
    return OOM();
  }
  defer.Add(std::bind(Pex_StartSearchResult_Delete, &result));

  auto buffer = Pex_Buffer_New();
  if (!buffer) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Buffer_Delete, &buffer));

  Pex_Buffer_Set(buffer, ft_->bytes().data(), ft_->bytes().size());

  Pex_StartSearchRequest_SetFingerprint(request, buffer, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }

  Pex_StartSearch(client_, request, result, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }

  size_t lookup_id_pos = 0;
  const char* lookup_id = nullptr;

  while (Pex_StartSearchResult_NextLookupID(result, &lookup_id_pos, &lookup_id)) {
    lookup_ids_.push_back(lookup_id);
  }
}

void SearchWorker::ExecuteCheckSearch() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  auto request = Pex_CheckSearchRequest_New();
  if (!request) {
    return OOM();
  }
  defer.Add(std::bind(Pex_CheckSearchRequest_Delete, &request));

  for (const auto& lookup_id : lookup_ids_) {
    Pex_CheckSearchRequest_AddLookupID(request, lookup_id.data());
  }

  result_ = Pex_CheckSearchResult_New();
  if (!result_) {
    return OOM();
  }

  Pex_CheckSearch(client_, request, result_, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }
}

Napi::Value SearchWorker::Resolve() {
  auto result = Napi::String::New(Env(), Pex_CheckSearchResult_GetJSON(result_));

  auto json = Env().Global().Get("JSON").As<Napi::Object>();
  auto parse = json.Get("parse").As<Napi::Function>();

  return parse.Call(json, {result});
}
