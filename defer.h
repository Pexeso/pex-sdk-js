// Copyright 2023 Pexeso Inc. All rights reserved.

#ifndef _DEFER_H_
#define _DEFER_H_

#include <functional>
#include <vector>

class Defer final {
 public:
  ~Defer();
  void Add(std::function<void()> fn);

 private:
  std::vector<std::function<void()>> fns_;
};

#endif  // _DEFER_H_
