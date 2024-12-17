include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(exosourcing_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(exosourcing_setup_options)
  option(exosourcing_ENABLE_HARDENING "Enable hardening" ON)
  option(exosourcing_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    exosourcing_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    exosourcing_ENABLE_HARDENING
    OFF)

  exosourcing_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR exosourcing_PACKAGING_MAINTAINER_MODE)
    option(exosourcing_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(exosourcing_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(exosourcing_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(exosourcing_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(exosourcing_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(exosourcing_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(exosourcing_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(exosourcing_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(exosourcing_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(exosourcing_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(exosourcing_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(exosourcing_ENABLE_PCH "Enable precompiled headers" OFF)
    option(exosourcing_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(exosourcing_ENABLE_IPO "Enable IPO/LTO" ON)
    option(exosourcing_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(exosourcing_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(exosourcing_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(exosourcing_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(exosourcing_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(exosourcing_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(exosourcing_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(exosourcing_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(exosourcing_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(exosourcing_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(exosourcing_ENABLE_PCH "Enable precompiled headers" OFF)
    option(exosourcing_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      exosourcing_ENABLE_IPO
      exosourcing_WARNINGS_AS_ERRORS
      exosourcing_ENABLE_USER_LINKER
      exosourcing_ENABLE_SANITIZER_ADDRESS
      exosourcing_ENABLE_SANITIZER_LEAK
      exosourcing_ENABLE_SANITIZER_UNDEFINED
      exosourcing_ENABLE_SANITIZER_THREAD
      exosourcing_ENABLE_SANITIZER_MEMORY
      exosourcing_ENABLE_UNITY_BUILD
      exosourcing_ENABLE_CLANG_TIDY
      exosourcing_ENABLE_CPPCHECK
      exosourcing_ENABLE_COVERAGE
      exosourcing_ENABLE_PCH
      exosourcing_ENABLE_CACHE)
  endif()

  exosourcing_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (exosourcing_ENABLE_SANITIZER_ADDRESS OR exosourcing_ENABLE_SANITIZER_THREAD OR exosourcing_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(exosourcing_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(exosourcing_global_options)
  if(exosourcing_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    exosourcing_enable_ipo()
  endif()

  exosourcing_supports_sanitizers()

  if(exosourcing_ENABLE_HARDENING AND exosourcing_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR exosourcing_ENABLE_SANITIZER_UNDEFINED
       OR exosourcing_ENABLE_SANITIZER_ADDRESS
       OR exosourcing_ENABLE_SANITIZER_THREAD
       OR exosourcing_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${exosourcing_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${exosourcing_ENABLE_SANITIZER_UNDEFINED}")
    exosourcing_enable_hardening(exosourcing_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(exosourcing_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(exosourcing_warnings INTERFACE)
  add_library(exosourcing_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  exosourcing_set_project_warnings(
    exosourcing_warnings
    ${exosourcing_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(exosourcing_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    exosourcing_configure_linker(exosourcing_options)
  endif()

  include(cmake/Sanitizers.cmake)
  exosourcing_enable_sanitizers(
    exosourcing_options
    ${exosourcing_ENABLE_SANITIZER_ADDRESS}
    ${exosourcing_ENABLE_SANITIZER_LEAK}
    ${exosourcing_ENABLE_SANITIZER_UNDEFINED}
    ${exosourcing_ENABLE_SANITIZER_THREAD}
    ${exosourcing_ENABLE_SANITIZER_MEMORY})

  set_target_properties(exosourcing_options PROPERTIES UNITY_BUILD ${exosourcing_ENABLE_UNITY_BUILD})

  if(exosourcing_ENABLE_PCH)
    target_precompile_headers(
      exosourcing_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(exosourcing_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    exosourcing_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(exosourcing_ENABLE_CLANG_TIDY)
    exosourcing_enable_clang_tidy(exosourcing_options ${exosourcing_WARNINGS_AS_ERRORS})
  endif()

  if(exosourcing_ENABLE_CPPCHECK)
    exosourcing_enable_cppcheck(${exosourcing_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(exosourcing_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    exosourcing_enable_coverage(exosourcing_options)
  endif()

  if(exosourcing_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(exosourcing_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(exosourcing_ENABLE_HARDENING AND NOT exosourcing_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR exosourcing_ENABLE_SANITIZER_UNDEFINED
       OR exosourcing_ENABLE_SANITIZER_ADDRESS
       OR exosourcing_ENABLE_SANITIZER_THREAD
       OR exosourcing_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    exosourcing_enable_hardening(exosourcing_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
