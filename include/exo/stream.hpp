#pragma once

//  Event Operations
// - append_event(stream_id, event) : Add an event to a specific stream.
// - fetch_event(event_id) : Retrieve a specific event by its unique ID.
// - fetch_events(criteria) : Retrieve a list of events filtered by criteria (e.g., time range, event type).

// Stream Operations
// - create_stream(stream_id, metadata) : Initialize a new stream with optional metadata.
// - fetch_stream_events(stream_id, from_position, limit) : Retrieve events for a specific stream, optionally paginated.
// - fetch_stream_metadata(stream_id) : Retrieve metadata for a specific stream.
// - delete_stream(stream_id) : Remove a stream.
// - subscribe_to_stream(stream_id, callback):  Subscribe to a stream for real-time event notifications.

#include <vector>

#include <exo/event.hpp>
#include <exo/store.hpp>

namespace exo {
  template<typename T>
  struct MemoryStoreInspector {
    static std::vector<Event<T>> get_events(const exo::MemoryStore<T>& store) {
      std::vector<Event<T>> copy = store.events;
      return copy;
    }
  };

  template<typename T, typename S>
  struct TypedStream {
    std::vector<Event<T>> get_all() const;

    protected:
    S store = {};
  };

  template<typename T>
  struct TypedStream<T, exo::MemoryStore<T>> {
    explicit TypedStream(exo::MemoryStore<T>& store) : store(store) {}

    std::vector<Event<T>> get_all() const {
      return exo::MemoryStoreInspector<T>::get_events(this->store);
    }

    protected:
    // NOTE: May be fine to stick with reference if we can prove ownership
    exo::MemoryStore<T>& store;
  };

  template<typename T, typename S>
  exo::TypedStream<T, S> make_typed_stream(S& store);

  template<typename T>
  exo::TypedStream<T, exo::MemoryStore<T>> make_typed_stream(exo::MemoryStore<T>& store) {
    return TypedStream<T, exo::MemoryStore<T>>(store);
  }
}
