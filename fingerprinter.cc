// Copyright 2023 Pexeso Inc. All rights reserved.

#include "fingerprint.h"
#include "fingerprinter.h"
#include "fingerprintworker.h"

int GetFingerprintTypes(const Napi::CallbackInfo& info) {
  int ft_types = 0;
  if (info.Length() > 1) {
    if (!info[1].IsArray()) {
      Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
      return 0;
    }

    auto array = info[1].As<Napi::Array>();
    for (uint32_t i = 0; i < array.Length(); i++) {
      Napi::Value val = array[i];
      if (!val.IsString()) {
        Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
        return 0;
      }

      std::string str = val.ToString();
      if (str == kAudio) {
        ft_types |= Pex_Fingerprint_Type_Audio;
      } else if (str == kVideo) {
        ft_types |= Pex_Fingerprint_Type_Video;
      } else if (str == kMelody) {
        ft_types |= Pex_Fingerprint_Type_Melody;
      } else if (str == kPhonetic) {
        ft_types |= Pex_Fingerprint_Type_Phonetic;
      } else if (str == kClassification) {
        ft_types |= Pex_Fingerprint_Type_Class;
      }
    }
  }

  if (ft_types > 0) {
    return ft_types;
  }
  return Pex_Fingerprint_Type_All;
}

Napi::Value Fingerprinter::FingerprintFile(const Napi::CallbackInfo& info) {
  if (info.Length() == 0 || !info[0].IsString()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto arg = info[0].As<Napi::String>();
  std::string str(arg);

  auto ft_types = GetFingerprintTypes(info);
  if (info.Env().IsExceptionPending()) {
    return info.Env().Undefined();
  }

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new FingerprintWorker(d, client_, std::move(str), true, ft_types);
  w->Queue();
  return d.Promise();
}

Napi::Value Fingerprinter::FingerprintBuffer(const Napi::CallbackInfo& info) {
  if (info.Length() == 0 || !info[0].IsBuffer()) {
    Napi::Error::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    return info.Env().Undefined();
  }

  auto arg = info[0].As<Napi::Uint8Array>();
  std::string_view buf(reinterpret_cast<char*>(arg.Data()), arg.ByteLength());

  auto ft_types = GetFingerprintTypes(info);
  if (info.Env().IsExceptionPending()) {
    return info.Env().Undefined();
  }

  auto d = Napi::Promise::Deferred::New(info.Env());
  auto w = new FingerprintWorker(d, client_, buf, false, ft_types);
  w->Queue();
  return d.Promise();
}
