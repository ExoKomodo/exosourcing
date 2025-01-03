#include <catch2/catch_test_macros.hpp>

#include <exo/event.hpp>
#include <exo/store.hpp>
#include <exo/stream.hpp>

#define PREFIX "[exo::Store]"

TEST_CASE("Can create memory store", "[exo::MemoryStore]")
{
  const exo::MemoryStore<int> store;
}

TEST_CASE("Can insert events into a memory store", "[exo::MemoryStore]")
{
  exo::MemoryStore<int> store;
  REQUIRE(store.emplace_back(exo::make_event<int>(1)));
  REQUIRE(store.emplace_back(exo::make_event<int>(2)));

  const auto events = exo::MemoryStoreInspector<int>::get_events(store);
  REQUIRE(events.at(0) == events.at(0));
  REQUIRE(events.at(1) == events.at(1));

  REQUIRE_FALSE(exo::make_event<int>(1) == events.at(0));
  REQUIRE_FALSE(exo::make_event<int>(2) == events.at(1));
  REQUIRE_FALSE(exo::make_event<int>(1) == events.at(1));
  REQUIRE_FALSE(exo::make_event<int>(2) == events.at(0));
}

