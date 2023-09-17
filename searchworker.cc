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

static Napi::Value ConvertSegmentType(Napi::Env env, int segment_type) {
  switch (segment_type) {
    case 1:
      return Napi::String::New(env, kAudio);
    case 2:
      return Napi::String::New(env, kVideo);
    case 3:
      return Napi::String::New(env, kMelody);
  }
  return Napi::String::New(env, "unknown");
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
  auto js_result = Napi::Object::New(Env());

  auto js_lookup_ids = Napi::TypedArray::New(Env());
  for (size_t i = 0; i < lookup_ids_.size(); i++) {
    js_lookup_ids[i] = lookup_ids_[i];
  }
  js_result.Set("lookup_ids", js_lookup_ids);

  auto js_matches = Napi::Array::New(Env());

  if (Pex_CheckSearchResult_MatchCount(result_) == 0) {
    js_result.Set("matches", js_matches);
    js_result.Freeze();
    return js_result;
  }

  Defer defer;

  auto status = Pex_Status_New();
  if (!status) {
    OOM();
    return Env().Undefined();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  auto match = Pex_SearchMatch_New();
  if (!match) {
    OOM();
    return Env().Undefined();
  }
  defer.Add(std::bind(Pex_SearchMatch_Delete, &match));

  auto asset = Pex_Asset_New();
  if (!asset) {
    OOM();
    return Env().Undefined();
  }
  defer.Add(std::bind(Pex_Asset_Delete, &asset));

  int matches_pos = 0;
  while (Pex_CheckSearchResult_NextMatch(result_, match, &matches_pos)) {
    auto js_segments = Napi::Array::New(Env());

    int64_t query_start;
    int64_t query_end;
    int64_t asset_start;
    int64_t asset_end;
    int type;

    int segments_pos = 0;
    while (Pex_SearchMatch_NextSegment(match, &query_start, &query_end, &asset_start, &asset_end,
                                       &type, &segments_pos)) {
      auto js_segment = Napi::Object::New(Env());
      js_segment["query_start"] = query_start;
      js_segment["query_end"] = query_end;
      js_segment["asset_start"] = asset_start;
      js_segment["asset_end"] = asset_end;
      js_segment["type"] = ConvertSegmentType(Env(), type);
      js_segment.Freeze();
      js_segments[segments_pos - 1] = js_segment;
    }

    Pex_SearchMatch_GetAsset(match, asset, status);
    if (!Pex_Status_OK(status)) {
      Fail(status);
      return Env().Undefined();
    }

    auto js_asset = Napi::Object::New(Env());
    js_asset["title"] = Pex_Asset_GetTitle(asset);
    js_asset["artist"] = Pex_Asset_GetArtist(asset);
    js_asset["label"] = Pex_Asset_GetLabel(asset);
    js_asset["duration"] = Pex_Asset_GetDuration(asset);
    js_asset.Freeze();

    auto js_match = Napi::Object::New(Env());
    js_match["asset"] = js_asset;
    js_match["segments"] = js_segments;
    js_match.Freeze();
    js_matches[matches_pos - 1] = js_match;
  }

  js_result["matches"] = js_matches;
  js_result.Freeze();
  return js_result;
}
