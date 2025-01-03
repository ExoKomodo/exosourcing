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
  REQUIRE_PARALLEL(100'000, 100, event.metadata.created < NOW,
    const auto event = exo::make_event<int>(1);
    std::this_thread::sleep_until(NOW + std::chrono::microseconds(1));
  );
}
