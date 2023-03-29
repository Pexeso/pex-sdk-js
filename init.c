// Copyright 2023 Pexeso Inc. All rights reserved.

#include <node_api.h>

#include "pexsearch.h"

NAPI_MODULE_INIT() {
  napi_value ret = NULL;
  napi_status status;

  status = napi_create_object(env, &ret);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to create object");
    return NULL;
  }

  status = napi_set_named_property(env, ret, "PexSearchClient",
                                   PexSearchClient(env));
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to set named property");
    return NULL;
  }

  return ret;
}
