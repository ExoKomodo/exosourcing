#pragma once

#include <chrono>
#include <exo/id.hpp>
#include <iostream>

namespace exo {
  template<typename T>
  struct Event;

  static IdGenerator id_generator = exo::make_id_generator();

  struct EventMetadata {
    exo::Id id;
    std::chrono::high_resolution_clock::time_point created;

    bool operator==(const EventMetadata& other) const {
      return this->id == other.id;
    }

    template <typename T>
    bool operator==(const Event<T>& other) const {
      return this == other.metadata;
    }
  };

  template<typename T>
  struct Event {
    EventMetadata metadata;
    T data;

    bool operator==(const Event<T>& other) const {
      return this->metadata == other.metadata;
    }

    bool operator==(const EventMetadata& other) const {
      return this->metadata == other;
    }
  };

  template<typename T>
  Event<T> make_event(T data) {
    return {
      .metadata = {
        .id = exo::id_generator(),
        .created = std::chrono::high_resolution_clock::now(),
      },
      .data = data,
    };
  }
}
