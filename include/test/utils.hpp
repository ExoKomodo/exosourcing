#pragma once

#include <chrono>
#include <thread>

#define NOW (std::chrono::high_resolution_clock::now())

#define REQUIRE_PARALLEL(thread_count, failure_check) do { \
  std::vector<std::thread> threads; \
  std::atomic<int> failures = { 0 }; \
  for (auto i = 0; i < (thread_count); i++) { \
    threads.emplace_back([&failures]() { \
      const auto event = exo::make_event<int>(1); \
      std::this_thread::sleep_until(NOW + std::chrono::nanoseconds(1)); \
      if ((failure_check)) { ++failures; } \
    }); \
  } \
  for (auto& t : threads) { t.join(); } \
  REQUIRE(failures == 0); \
} while (false);
