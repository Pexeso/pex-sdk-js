// Copyright 2023 Pexeso Inc. All rights reserved.

#include "pexsearch.h"

#include <stdio.h>

#include <pex/ae/sdk/client.h>
#include <pex/ae/sdk/init.h>
#include <pex/ae/sdk/lock.h>
#include <pex/ae/sdk/status.h>

#include "defer.h"

static const int kMaxArgSize = 100;
static const char *kClientProperty = "#client";

static napi_status ArgsToCreds(napi_env env, napi_callback_info info,
                               napi_value *this, char *client_id,
                               char *client_secret) {
  size_t argc = 2;
  napi_value argv[2];

  napi_status status = napi_get_cb_info(env, info, &argc, argv, this, NULL);
  if (status != napi_ok) {
    return napi_invalid_arg;
  }

  if (argc != 2) {
    return napi_invalid_arg;
  }

  size_t result;
  status =
      napi_get_value_string_utf8(env, argv[0], client_id, kMaxArgSize, &result);
  if (status != napi_ok) {
    return napi_invalid_arg;
  }

  status = napi_get_value_string_utf8(env, argv[1], client_secret, kMaxArgSize,
                                      &result);
  if (status != napi_ok) {
    return napi_invalid_arg;
  }

  return napi_ok;
}

static void Destructor(napi_env env, void *data, void *hint) {
  printf("starting destructor\n");
  AE_Lock();
  AE_Client_Delete(data);
  AE_Unlock();
  AE_Cleanup();
  printf("starting finishing destructor\n");
}

static napi_value Constructor(napi_env env, napi_callback_info info) {
  DEFER_INIT();

  napi_value this;
  char client_id[kMaxArgSize];
  char client_secret[kMaxArgSize];

  napi_status status = ArgsToCreds(env, info, &this, client_id, client_secret);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "invalid arguments");
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

  AE_Lock();
  DEFER0(AE_Unlock);

  AE_Status *ae_status = AE_Status_New();
  if (!ae_status) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }
  DEFER1(AE_Status_Delete, &ae_status);

  AE_Client *ae_client = AE_Client_New();
  if (!ae_client) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }

  AE_Client_InitType(ae_client, AE_PEX_SEARCH, client_id, client_secret,
                     ae_status);
  if (!AE_Status_OK(ae_status)) {
    AE_Client_Delete(&ae_client);
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }

  napi_value client;
  status = napi_create_object(env, &client);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "out of memory");
    return NULL;
  }

  status = napi_wrap(env, client, ae_client, Destructor, NULL, NULL);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to wrap client object");
    return NULL;
  }

  status = napi_set_named_property(env, this, kClientProperty, client);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "failed to set property");
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
