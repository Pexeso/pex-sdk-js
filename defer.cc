// Copyright 2023 Pexeso Inc. All rights reserved.

#include "defer.h"

void Defer::Add(std::function<void()> fn) {
  fns_.push_back(std::move(fn));
}

Defer::~Defer() {
  for (auto it = fns_.rbegin(); it != fns_.rend(); it++) {
    (*it)();
  }
}
