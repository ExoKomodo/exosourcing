#include <optional>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <exosourcing/exosourcing.hpp>

// This file will be generated automatically when you run the CMake
// configuration step. It creates a namespace called `exosourcing`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

namespace {
  int parseArgs(int argc, const char **argv, CLI::App &app)
  {
    CLI11_PARSE(app, argc, argv);
    return 0;
  }
}

int main(int argc, const char **argv)
{
  try {
    std::optional<std::string> message;
    bool show_version = false;
    {
      CLI::App app{ fmt::format(
        "{} version {}", exosourcing::cmake::project_name, exosourcing::cmake::project_version) };
      app.add_option("-m,--message", message, "A message to print back out");
      app.add_flag("--version", show_version, "Show version information");
      if (const auto exitCode = parseArgs(argc, argv, app); exitCode != 0) { return exitCode; }
      CLI11_PARSE(app, argc, argv);
    }

    if (show_version) {
      fmt::print("{} {} {}\n",
        exosourcing::cmake::project_name,
        exosourcing::cmake::project_version,
        exosourcing::cmake::git_short_sha);
    }
    if (message) { spdlog::info("Some message: {}", message.value()); }

    static_assert(factorial_constexpr(5) == 120);
    assert(factorial(5) == 120);

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
    return 1;
  }
  return EXIT_SUCCESS;
}
