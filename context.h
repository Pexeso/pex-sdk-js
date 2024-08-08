// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <napi.h>

struct Context {
  Napi::FunctionReference pexsearch;
  Napi::FunctionReference privatesearch;
  Napi::FunctionReference fingerprint;
  Napi::FunctionReference error;
  Napi::FunctionReference lister;
};

#endif  // _CONTEXT_H
