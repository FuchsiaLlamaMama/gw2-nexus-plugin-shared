# gw2-nexus-plugin-shared

Shared C++17 helpers for Guild Wars 2 addons built on [Raidcore Nexus](https://raidcore.gg/gw2/nexus).

- **`persistence/`**: crash-safe atomic file writes (`atomic_write`, `read_file`).
- **`theme/`**: a GW2-native look for ImGui panels. Includes design tokens (`theme.h`), 9-slice frame geometry (`nine_slice.h`), and ImGui helpers (`theme_imgui.h`): panel style, title bar, themed frame.

`persistence/` and the token/geometry parts of `theme/` have no Nexus, ImGui or Windows dependency, so they build and unit-test on macOS/Linux as well as MSVC. `theme_imgui.h` is header-only and needs `imgui.h` on the include path.

## Use

Add it as a git submodule and pull it into your CMake build:

```bash
git submodule add https://github.com/FuchsiaLlamaMama/gw2-nexus-plugin-shared shared
```

```cmake
add_subdirectory(shared)
target_link_libraries(my-addon PRIVATE shared-core)
```

Headers are included relative to the repo root, e.g. `#include "theme/theme.h"`. Everything lives in the `shared::` namespace.

The tests (`tests/test_theme.cpp`) build when your project defines a `doctest` target before `add_subdirectory(shared)`.

## License

MIT. See [LICENSE](LICENSE).
