#include "trace.h"
#include <atomic>
#include <ctime>
#include <iostream>

namespace trace {
static Stats g_stats;

Stats &stats() { return g_stats; }

uint64_t now_ns() {
  timespec ts{};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

void emit_alloc(const AllocEvent &event) {
  g_stats.alloc_count++;
  g_stats.live_bytes += event.bytes;
  g_stats.peak_bytes = std::max(g_stats.peak_bytes, g_stats.live_bytes);

  std::printf("[ALLOC] id=%llu tag=%s bytes=%zu live=%zu peak=%zu\n",
              (unsigned long long)event.id, event.tag, event.bytes,
              g_stats.live_bytes, g_stats.peak_bytes);
}

void emit_free(const FreeEvent &event) {
  g_stats.free_count++;
  if (g_stats.live_bytes > event.bytes)
    g_stats.live_bytes -= event.bytes;
  std::printf("[FREE] id=%llu tag=%s bytes=%zu live=%zu\n",
              (unsigned long long)event.id, event.tag, event.bytes,
              g_stats.live_bytes);
}

void emit_op(const OpEvent &event) {
  g_stats.op_count++;
  g_stats.total_op_us += event.duration_us;
  std::printf("[OP] id=%llu name=%s duration=%f bytes_in=%zu bytes_out=%zu\n",
              (unsigned long long)event.op_id, event.name,
              event.duration_us / 1000.0, event.bytes_in, event.bytes_out);
}

void dump_stats() {
  std::cout << "\n==== RUNTIME STATS ====\n";
  std::cout << "alloc_count=%llu free_count=%llu\n",
      (unsigned long long)g_stats.alloc_count,
      (unsigned long long)g_stats.free_count;
  std::cout << "live_bytes=%zu peak_bytes=%zu\n", g_stats.live_bytes,
      g_stats.peak_bytes;
  std::cout << "op_count=%llu total_op_us=%.2f avg_op_us=%.2f\n",
      (unsigned long long)g_stats.op_count, g_stats.total_op_us,
      (g_stats.op_count ? g_stats.total_op_us / (double)g_stats.op_count : 0.0);
  std::cout << "=======================\n";
}

ScopedTimer::ScopedTimer(const char *n, uint64_t id)
    : name(n), op_id(id), start_ns(now_ns()) {}

ScopedTimer::~ScopedTimer() {
  const uint64_t end_ns = now_ns();
  const double us = (double)(end_ns - start_ns) / 1000.0;
  emit_op(OpEvent{op_id, name, us, bytes_in, bytes_out});
}
} // namespace trace
