add_executable(demo main.cpp)

target_link_libraries(
  demo
  PRIVATE exosourcing::exosourcing_options
          exosourcing::exosourcing_warnings)

target_link_system_libraries(
  demo
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
          lefticus::tools)

target_include_directories(demo PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
