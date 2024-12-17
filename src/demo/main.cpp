// #include <array>
// #include <functional>
// #include <iostream>
// #include <optional>

// #include <random>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

// #include <lefticus/tools/non_promoting_ints.hpp>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `exosourcing`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

static int parseArgs(int argc, const char **argv, CLI::App &app)
{
  CLI11_PARSE(app, argc, argv);
  return 0;
}

int main([[maybe_unused]] int x, [[maybe_unused]] const char **argv)
{
  try {
    std::optional<std::string> message;
    bool show_version = false;
    {
      CLI::App app{ fmt::format(
        "{} version {}", exosourcing::cmake::project_name, exosourcing::cmake::project_version) };
      app.add_option("-m,--message", message, "A message to print back out");
      app.add_flag("--version", show_version, "Show version information");
      const auto exitCode = parseArgs(x, argv, app);
      if (exitCode != 0) { return exitCode; }
      CLI11_PARSE(app, x, argv);
    }

    if (show_version) {
      fmt::print("{} {} Commit\n",
        exosourcing::cmake::project_name,
        exosourcing::cmake::project_version,
        exosourcing::cmake::git_short_sha);
    }
    if (message) { spdlog::info("Some message: {}", message.value()); }
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
    return 1;
  }
  return EXIT_SUCCESS;
}
