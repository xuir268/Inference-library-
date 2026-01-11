#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <sys/types.h>

namespace trace {

struct AllocEvent {
  uint64_t id;
  const char *tag;
  size_t bytes;
};

struct FreeEvent {
  uint64_t id;
  const char *tag;
  size_t bytes;
};

struct OpEvent {
  uint64_t op_id;
  const char *name;
  double duration_us;
  size_t bytes_in;
  size_t bytes_out;
};

struct Stats {
  size_t live_bytes;
  size_t peak_bytes;
  uint64_t alloc_count;
  uint64_t free_count;
  uint64_t op_count;
  double total_op_us;
};

Stats &stats();
void emit_alloc(const AllocEvent &event);
void emit_free(const FreeEvent &event);
void emit_op(const OpEvent &event);

void dump_stats();

struct ScopedTimer {
  const char *name;
  uint64_t op_id;
  size_t bytes_in = 0;
  size_t bytes_out = 0;

  explicit ScopedTimer(const char *n, uint64_t id);
  ~ScopedTimer();

private:
  uint64_t start_ns;
};

uint64_t now_ns();
} // namespace trace