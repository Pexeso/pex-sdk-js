// Copyright 2023 Pexeso Inc. All rights reserved.

#include "listworker.h"

#include <pex/sdk/lock.h>
#include <pex/sdk/ingestion.h>

#include "defer.h"

void ListWorker::Execute() {
  Defer defer;

  Pex_Lock();
  defer.Add(Pex_Unlock);

  auto req = Pex_ListRequest_New();
  if (!req) {
    return OOM();
  }
  defer.Add(std::bind(Pex_ListRequest_Delete, &req));

  auto res = Pex_ListResult_New();
  if (!req) {
    return OOM();
  }
  defer.Add(std::bind(Pex_ListResult_Delete, &res));

  auto status = Pex_Status_New();
  if (!status) {
    return OOM();
  }
  defer.Add(std::bind(Pex_Status_Delete, &status));

  Pex_ListRequest_SetAfter(req, end_cursor_->c_str());
  Pex_ListRequest_SetLimit(req, limit_);

  Pex_List(client_, req, res, status);
  if (!Pex_Status_OK(status)) {
    return Fail(status);
  }

  result_json_ = Pex_ListResult_GetJSON(res);
}

Napi::Value ListWorker::Resolve() {
  auto result = Napi::String::New(Env(), result_json_);

  auto json = Env().Global().Get("JSON").As<Napi::Object>();
  auto parse = json.Get("parse").As<Napi::Function>();

  Napi::Object parsed = parse.Call(json, {result}).ToObject();

  *end_cursor_ = parsed.Get("end_cursor").ToString();
  *has_next_page_ = parsed.Get("has_next_page").ToBoolean();
  return parsed.Get("entries");
}
