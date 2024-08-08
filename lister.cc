
// Copyright 2024 Pexeso Inc. All rights reserved.

#include "lister.h"

#include "listworker.h"
#include "context.h"

Napi::Value Lister::New(Napi::Env env, Pex_Client* client, std::string end_cursor, int limit) {
  auto ctx = env.GetInstanceData<Context>();
  auto lister = ctx->lister.New({});
  auto raw = Napi::ObjectWrap<Lister>::Unwrap(lister);

  raw->client_ = client;
  raw->end_cursor_ = std::move(end_cursor);
  raw->limit_ = limit;

  return lister;
}

Napi::Object Lister::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Lister",
                                    {
                                        InstanceMethod("list", &Lister::List),
                                        InstanceMethod("endCursor", &Lister::EndCursor),
                                        InstanceMethod("hasNextPage", &Lister::HasNextPage),
                                    });

  auto ctx = env.GetInstanceData<Context>();
  ctx->lister = Napi::Persistent(func);

  exports.Set("Lister", func);
  return exports;
}

Napi::Value Lister::List(const Napi::CallbackInfo& info) {
  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new ListWorker(d, client_, &end_cursor_, &has_next_page_, limit_);
  w->Queue();
  return d.Promise();
}

Napi::Value Lister::EndCursor(const Napi::CallbackInfo& info) {
  return Napi::String::New(Env(), end_cursor_);
}

Napi::Value Lister::HasNextPage(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(Env(), has_next_page_);
}
