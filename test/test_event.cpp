#include <catch2/catch_test_macros.hpp>

#include <exo/event.hpp>

#define PREFIX "[exo::Event]"

TEST_CASE("Can create event", PREFIX)
{
  const exo::Event<int> event = {.data = 1};
  REQUIRE(event.data == 1);
}
