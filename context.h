// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <napi.h>

struct Context {
  Napi::FunctionReference pex_search;
  Napi::FunctionReference fingerprint;
};

#endif  // _CONTEXT_H
