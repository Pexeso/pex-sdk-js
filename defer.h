// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _DEFER_H_
#define _DEFER_H_

#include <node_api.h>

#define DEFER_INIT()                                                           \
  napi_value defer_;                                                           \
  napi_create_object(env, &defer_);

#define DEFER0(f) napi_add_finalizer(env, defer_, NULL, DeferFunc, f, NULL)
#define DEFER1(f, p)                                                           \
  napi_add_finalizer(env, defer_, p, DeferFuncArg, NULL, NULL)

void DeferFunc(napi_env env, void *finalize_data, void *finalize_hint) {
  void (*fn)() = finalize_hint;
  fn();
}

void DeferFuncArg(napi_env env, void *finalize_data, void *finalize_hint) {
  void (*fn)(void *) = finalize_hint;
  fn(finalize_data);
}

#endif // _DEFER_H_
