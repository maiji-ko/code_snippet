# AGENTS.md

## Build & Run

```sh
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

- Requires CMake >= 3.20.1 and a C++17 compiler. Default `CMAKE_BUILD_TYPE` is `Debug` (set in root `CMakeLists.txt`).
- Third-party deps (tomlplusplus, fast-cpp-csv-parser) are auto-downloaded by CMake `FetchContent` on first configure — first build needs network access.
- **Run the binary from the repo root.** `src/main.cpp:17` resolves the config path as `fs::current_path() / "config" / "config.toml"`, and `ConfigParser::load` (`src/tomlplusplus/tomlplusplus.cpp:12`) sets `m_projectRoot` from `fs::current_path()`. Running from `build/` or any other directory will fail to find `config/config.toml` and all `get_abs_path()` calls will return wrong paths.

## Project Structure

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — loads `config/config.toml`, prints config, calls `sample_fsv()` |
| `src/fast-cpp-csv-parser/` | Local wrapper; `fast-cpp-csv-parser.cpp` defines `int sample_fsv(const std::string& csv_path)` |
| `src/tomlplusplus/` | Local wrapper; `ConfigParser` class with `load`, `get_string`, `get_abs_path`, etc. |
| `src/CMakeLists.txt` | Executable target `snippet_executable`; lists wrapper `.cpp` files explicitly |
| `third-part/CMakeLists.txt` | `FetchContent` declarations; exposes `fast_csv` (INTERFACE) and links `tomlplusplus_tomlplusplus` |
| `config/config.toml` | Runtime config; CSV input/output paths and project name/version |
| `data/fast-cpp-csv-parser/in/ram.csv` | Sample CSV (3 columns: vendor, size, speed) |

## Adding Code

- New source files must be added to the `add_executable(...)` list in `src/CMakeLists.txt:3-7` — sources are listed explicitly, not globbed.
- The wrapper `src/tomlplusplus/tomlplusplus.cpp` and `src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp` are compiled **into the executable** (not a library). Their public API is exposed via matching `.h` headers included from `src/main.cpp`.
- tomlplusplus needs C++17 (used by the `toml++/toml.hpp` header); fast-cpp-csv-parser only needs C++11 (declared in `third-part/CMakeLists.txt:30`).

## VS Code

`.vscode/` provides tasks (`cmake-configure` uses `--fresh`, `cmake-build` is default, `run` is default test task) and a GDB launch config that auto-runs `cmake-build` and sets `cwd` to `${workspaceFolder}`. F5 works out of the box.

## Branch Workflow

- `main` is the release branch.
- `dev` is the integration branch. PRs merge `dev` into `main`.
- Work on feature branches off `dev`.

## Testing

No test framework is set up. Run the binary and check stdout/stderr manually. The expected CSV path is loaded from `config/config.toml` under `[data] fsv_test_file_in`.
