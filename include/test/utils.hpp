#pragma once

#include <algorithm>
#include <chrono>
#include <thread>

#define NOW (std::chrono::high_resolution_clock::now())

#define REQUIRE_PARALLEL(iterations, max_threads, check_condition, body) do { \
  std::atomic<int> failures = { 0 }; \
  auto remaining = (iterations); \
  while (remaining > 0) { \
    std::vector<std::thread> threads; \
    for (auto i = 0; i < std::min((max_threads), remaining); i++) { \
      threads.emplace_back([&failures]() { \
        do { body; if (!(check_condition)) { ++failures; } } while (false); \
      }); \
      remaining--; \
    } \
    for (auto& t : threads) { t.join(); } \
  } \
  REQUIRE(failures == 0); \
} while (false);
