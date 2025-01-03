#include <catch2/catch_test_macros.hpp>

#include <exo/event.hpp>
#include <test/utils.hpp>

#define PREFIX "[exo::Event]"


TEST_CASE("Can create event", PREFIX)
{
  const exo::Event<int> event = exo::make_event(1);
  REQUIRE(event.data == 1);
}

TEST_CASE("Can compare events with ==", PREFIX)
{
  const exo::Event<int> event = exo::make_event(1);
  const exo::Event<int> other = exo::make_event(1);
  REQUIRE_FALSE(event == other);
}

TEST_CASE("Creation time is in the past", PREFIX)
{
  REQUIRE_PARALLEL(10'000, event.metadata.created >= NOW);
}
