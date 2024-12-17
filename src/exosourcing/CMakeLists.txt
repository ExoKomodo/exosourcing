include(GenerateExportHeader)

add_library(exosourcing exosourcing.cpp)

add_library(exosourcing::exosourcing ALIAS exosourcing)

target_link_libraries(exosourcing PRIVATE exosourcing_options exosourcing_warnings)

target_include_directories(exosourcing ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(exosourcing PUBLIC cxx_std_20)

set_target_properties(
  exosourcing
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(exosourcing EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/exosourcing/exosourcing_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(exosourcing PUBLIC EXOSOURCING_STATIC_DEFINE)
endif()
