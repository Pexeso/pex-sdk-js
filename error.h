// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _ERROR_H_
#define _ERROR_H_

#include <napi.h>

enum ErrorCode {
  OK = 0,
  DEADLINE_EXCEEDED = 1,
  PERMISSION_DENIED = 2,
  UNAUTHENTICATED = 3,
  NOT_FOUND = 4,
  INVALID_INPUT = 5,
  OUT_OF_MEMORY = 6,
  INTERNAL_ERROR = 7,
  NOT_INITIALIZED = 8,
  CONNECTION_ERROR = 9,
  LOOKUP_FAILED = 10,
  LOOKUP_TIMED_OUT = 11,
  RESOURCE_EXHAUSTED = 12
};

class Error : public Napi::ObjectWrap<Error> {
 public:
  static Napi::Value New(Napi::Env env, int code, std::string message, bool is_retryable = false);
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  Error(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Error>(info) {}
};

#endif  // _ERROR_H_
