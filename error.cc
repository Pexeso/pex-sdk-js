// Copyright 2023 Pexeso Inc. All rights reserved.

#include "error.h"

#include "context.h"

Napi::Value Error::New(Napi::Env env, int code, std::string message, bool is_retryable) {
  auto ctx = env.GetInstanceData<Context>();
  auto err = ctx->error.New({});
  err.Set("code", code);
  err.Set("message", message);
  err.Set("isRetryable", is_retryable);
  err.Freeze();
  return err;
}

Napi::Object Error::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(
      env, "Error",
      {
          StaticValue("DEADLINE_EXCEEDED", Napi::Number::New(env, ErrorCode::DEADLINE_EXCEEDED)),
          StaticValue("PERMISSION_DENIED", Napi::Number::New(env, ErrorCode::PERMISSION_DENIED)),
          StaticValue("UNAUTHENTICATED", Napi::Number::New(env, ErrorCode::UNAUTHENTICATED)),
          StaticValue("NOT_FOUND", Napi::Number::New(env, ErrorCode::NOT_FOUND)),
          StaticValue("INVALID_INPUT", Napi::Number::New(env, ErrorCode::INVALID_INPUT)),
          StaticValue("OUT_OF_MEMORY", Napi::Number::New(env, ErrorCode::OUT_OF_MEMORY)),
          StaticValue("INTERNAL_ERROR", Napi::Number::New(env, ErrorCode::INTERNAL_ERROR)),
          StaticValue("NOT_INITIALIZED", Napi::Number::New(env, ErrorCode::NOT_INITIALIZED)),
          StaticValue("CONNECTION_ERROR", Napi::Number::New(env, ErrorCode::CONNECTION_ERROR)),
          StaticValue("LOOKUP_FAILED", Napi::Number::New(env, ErrorCode::LOOKUP_FAILED)),
          StaticValue("LOOKUP_TIMED_OUT", Napi::Number::New(env, ErrorCode::LOOKUP_TIMED_OUT)),
          StaticValue("RESOURCE_EXHAUSTED", Napi::Number::New(env, ErrorCode::RESOURCE_EXHAUSTED)),
      });

  auto ctx = env.GetInstanceData<Context>();
  ctx->error = Napi::Persistent(func);

  exports.Set("Error", func);
  return exports;
}
