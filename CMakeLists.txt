# gw2-nexus-plugin-shared — cross-addon layer for GW2 Nexus plugins.
#
# Consumed as a git submodule and pulled in with add_subdirectory(). Two
# pure-C++17 pieces, no Nexus/ImGui/Windows, so they compile and unit-test on
# macOS/clang as well as MSVC:
#   persistence/ — the crash-safe write-through helper.
#   theme/       — the native-look theme layer: the ImGui-free design tokens
#                  (theme.h) + the 9-slice frame geometry (nine_slice). The
#                  ImGui glue that consumes these lives in the addon DLLs
#                  (Windows-only) via theme/theme_imgui.h.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.20)
  project(gw2-nexus-plugin-shared LANGUAGES CXX)
endif()

add_library(shared-core STATIC
  persistence/atomic_file.cpp
  theme/nine_slice.cpp
)
target_include_directories(shared-core PUBLIC .)
target_compile_features(shared-core PUBLIC cxx_std_17)

# --- shared-core tests (doctest, run via ctest) --------------------------------
# Built only when the consuming project provides a `doctest` target.
if(TARGET doctest)
  add_executable(shared-core-tests tests/test_theme.cpp)
  target_link_libraries(shared-core-tests PRIVATE shared-core doctest)
  add_test(NAME shared-core-tests COMMAND shared-core-tests)
endif()
