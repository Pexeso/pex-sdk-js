// Copyright 2023 Pexeso Inc. All rights reserved.

#include "pexsearch.h"

#include <stdio.h>

#include <pex/ae/sdk/init.h>
#include <pex/ae/sdk/status.h>

static napi_value Constructor(napi_env env, napi_callback_info info) {
  size_t argc = 2;
  napi_value argv[2];

  napi_status status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to get cb info");
    return NULL;
  }

  if (argc != 2) {
    napi_throw_error(env, NULL, "invalid number of arguments");
    return NULL;
  }

  char client_id[100];
  char client_secret[100];
  size_t result;
  status = napi_get_value_string_utf8(env, argv[0], client_id,
                                      sizeof(client_id), &result);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "invalid client_id");
    return NULL;
  }

  status = napi_get_value_string_utf8(env, argv[1], client_secret,
                                      sizeof(client_secret), &result);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "invalid client_secret");
    return NULL;
  }

  int status_code;
  char status_message[100];

  AE_Init(client_id, client_secret, &status_code, status_message,
          sizeof(status_message));
  if (status_code != 0) {
    napi_throw_error(env, NULL, "failed to initialize client");
    return NULL;
  }

  return NULL;
}

static napi_value StartSearch(napi_env env, napi_callback_info info) {
  printf("StartSearch\n");
  return NULL;
}

napi_value PexSearch(napi_env env) {
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
      napi_define_class(env, "PexSearch", NAPI_AUTO_LENGTH, Constructor, NULL,
                        1, properties, &cls);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to create string");
    return NULL;
  }
  return cls;
}
