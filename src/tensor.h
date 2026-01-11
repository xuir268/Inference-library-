#pragma once
#include "device.h"
#include <cstdint>

struct Tensor {
  int64_t rows;
  int64_t cols;
  Device device;
  float *data;
  size_t bytes;
  uint64_t alloc_id;
};

Tensor tensor_new(int64_t r, int64_t c, Device d);
void tensor_free(Tensor &t);