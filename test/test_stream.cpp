#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include <exo/event.hpp>
#include <exo/store.hpp>
#include <exo/stream.hpp>

#define PREFIX "[exo::Stream]"

TEST_CASE("Can read events from a stream", PREFIX)
{
  exo::MemoryStore<int> store;
  REQUIRE(store.emplace_back(exo::make_event<int>(1)));
  REQUIRE(store.emplace_back(exo::make_event<int>(2)));

  auto stream = exo::make_typed_stream(store);
  auto events = stream.get_all();
  REQUIRE(exo::make_event<int>(1) == events.at(0));
  REQUIRE(exo::make_event<int>(2) == events.at(1));

  REQUIRE_FALSE(exo::make_event<int>(1) == events.at(1));
  REQUIRE_FALSE(exo::make_event<int>(2) == events.at(0));
}

TEST_CASE("Can read new events from a stream", PREFIX)
{
  exo::MemoryStore<int> store;
  REQUIRE(store.emplace_back(exo::make_event<int>(1)));
  REQUIRE(store.emplace_back(exo::make_event<int>(2)));

  const auto stream = exo::make_typed_stream(store);
  {
    const auto events = stream.get_all();
    REQUIRE(events.size() == 2);
    REQUIRE(exo::make_event<int>(1) == events.at(0));
    REQUIRE(exo::make_event<int>(2) == events.at(1));

    REQUIRE_FALSE(exo::make_event<int>(1) == events.at(1));
    REQUIRE_FALSE(exo::make_event<int>(2) == events.at(0));
  }
  {
    const auto events = stream.get_all();
    REQUIRE(events.size() == 2);
    REQUIRE(store.emplace_back(exo::make_event<int>(3)));
  }
  {
    const auto events = stream.get_all();
    REQUIRE(events.size() == 3);

    REQUIRE(exo::make_event<int>(1) == events.at(0));
    REQUIRE(exo::make_event<int>(2) == events.at(1));
    REQUIRE(exo::make_event<int>(3) == events.at(2));

    REQUIRE_FALSE(exo::make_event<int>(1) == events.at(1));
    REQUIRE_FALSE(exo::make_event<int>(2) == events.at(2));
    REQUIRE_FALSE(exo::make_event<int>(3) == events.at(0));
  }
}
