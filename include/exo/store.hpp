#pragma once

#include <vector>

#include <exo/event.hpp>

namespace exo {
  // NOTE: Forward declare all friend types
  template<typename T>
  struct MemoryStoreInspector;

  template<typename T>
  struct Store {
    bool emplace_back(const exo::Event<T> event);
  };

  template<typename T>
  struct MemoryStore : exo::Store<T> {
    friend struct exo::MemoryStoreInspector<T>;

    bool emplace_back(const exo::Event<T> event) {
      try {
        this->events.emplace_back(event);
      } catch (const std::exception& e) {
        return false;
      }
      return true;
    }

    protected:
    // NOTE: Using a std::vector, since we always `emplace_back`,
    // allowing for guaranteed O(1) insertion
    std::vector<Event<T>> events;
  };

  template<typename T>
  exo::MemoryStore<T> make_memory_store() {
    return {};
  }
}
