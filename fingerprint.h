// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _FINGERPRINT_H_
#define _FINGERPRINT_H_

#include <napi.h>

const auto kAudio = "audio";
const auto kVideo = "video";
const auto kMelody = "melody";
const auto kPhonetic = "phonetic";
const auto kClassification = "classification";

class Fingerprint final : public Napi::ObjectWrap<Fingerprint> {
 public:
  static Napi::Value New(Napi::Env env, std::string bytes);
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Fingerprint(const Napi::CallbackInfo& info);

  const std::string& bytes() const { return bytes_; }
  void set_bytes(std::string bytes) { bytes_ = std::move(bytes); };

 private:
  Napi::Value Dump(const Napi::CallbackInfo& info);

  std::string bytes_;
};

#endif  // _FINGERPRINT_H_
