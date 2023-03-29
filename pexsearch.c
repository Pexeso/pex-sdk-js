// Copyright 2023 Pexeso Inc. All rights reserved.

#include "pexsearch.h"
#include <stdio.h>

static napi_value Constructor(napi_env env, napi_callback_info info) {
  printf("Constructor\n");
  return NULL;
}

static napi_value StartSearch(napi_env env, napi_callback_info info) {
  printf("StartSearch\n");
  return NULL;
}

napi_value PexSearchClient(napi_env env) {
  napi_value cls;

  napi_property_descriptor properties[] = {{
      .utf8name = "start_search",
      .name = NULL,
      .method = StartSearch,
      .getter = NULL,
      .setter = NULL,
      .value = NULL,
      .attributes = napi_default_method,
      .data = NULL,
  }};

  napi_status status =
      napi_define_class(env, "PexSearchClient", NAPI_AUTO_LENGTH, Constructor,
                        NULL, 1, properties, &cls);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to create string");
    return NULL;
  }
  return cls;
}
