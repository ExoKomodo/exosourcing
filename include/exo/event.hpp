#pragma once

#include <chrono>

namespace exo {
  template<typename T>
  struct Event {
    T data;

    bool operator==(const Event<T>& other) const {
      return this->data == other.data;
    }
  };

  template<typename T>
  Event<T> make_event(T data) {
    return {
      .data = data,
    };
  }
}
