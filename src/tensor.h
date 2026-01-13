#pragma once
#include <cstddef>
#include <cstdint>

enum class Device {
  CPU,
  // CUDA later
};

struct Tensor {
  int64_t rows = 0;
  int64_t cols = 0;
  Device device = Device::CPU;

  float *data = nullptr;
  size_t bytes = 0;

  uint64_t alloc_id = 0;
  const char *tag = "tensor"; // for observability

  int64_t numel() const { return rows * cols; }
};

Tensor tensor_new(int64_t rows, int64_t cols, const char *tag = "tensor");
void tensor_free(Tensor &t);

// helpers
void tensor_fill(Tensor &t, float v);
void tensor_iota(Tensor &t, float start = 0.0f, float step = 1.0f);