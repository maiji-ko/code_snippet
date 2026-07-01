# AGENTS.md

## Build & Run

```sh
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

- Requires CMake >= 3.20.1 and a C++17 compiler. Default `CMAKE_BUILD_TYPE` is `Debug` (set in root `CMakeLists.txt:5-7`).
- Third-party deps (tomlplusplus, fast-cpp-csv-parser, spdlog) are auto-downloaded by CMake `FetchContent` (`third-part/CMakeLists.txt`) on first configure — first build needs network access.
- **Run the binary from the repo root.** `src/main.cpp:18` resolves the config path as `fs::current_path() / "config" / "config.toml"`, and `ConfigParser::load` (`src/tomlplusplus/tomlplusplus.cpp:12`) sets `m_projectRoot` from `fs::current_path()`. Running from `build/` or any other directory fails to find `config/config.toml` and `get_abs_path()` returns wrong paths.
- **spdlog is linked as a shared library** (`SPDLOG_BUILD_SHARED=ON` in `third-part/CMakeLists.txt:42`). `src/CMakeLists.txt:17` sets `BUILD_RPATH` to `_deps/spdlog-build` so `./build/src/snippet_executable` runs without `LD_LIBRARY_PATH` — don't remove this when editing `src/CMakeLists.txt`.

## Project Structure

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — loads `config/config.toml`, prints config, calls `sample_fsv()` |
| `src/fast-cpp-csv-parser/` | Local wrapper; `fast-cpp-csv-parser.cpp` defines `int sample_fsv(const std::string& csv_path)` |
| `src/tomlplusplus/` | Local wrapper; `ConfigParser` class with `load`, `get_string`, `get_abs_path`, etc. |
| `src/CMakeLists.txt` | Executable target `snippet_executable`; lists wrapper `.cpp` files explicitly; links `fast_csv`, `tomlplusplus_tomlplusplus`, `spdlog::spdlog` |
| `third-part/CMakeLists.txt` | `FetchContent` declarations for tomlplusplus / fast-cpp-csv-parser / spdlog; exposes `fast_csv` (INTERFACE) |
| `config/config.toml` | Runtime config; CSV input/output paths and project name/version |
| `data/fast-cpp-csv-parser/in/ram.csv` | Sample CSV (3 columns: vendor, size, speed) |

## Adding Code

- New source files must be added to the `add_executable(...)` list in `src/CMakeLists.txt:3-7` — sources are listed explicitly, not globbed.
- The wrappers `src/tomlplusplus/tomlplusplus.cpp` and `src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp` are compiled **into the executable** (not a library). Their public API is exposed via matching `.h` headers included from `src/main.cpp`.
- New third-party deps: declare them via `FetchContent` in `third-part/CMakeLists.txt` and add the target to `target_link_libraries` in `src/CMakeLists.txt:9-14`. If the dep is a shared library, update `BUILD_RPATH` (see `src/CMakeLists.txt:16-19`) so the binary runs without `LD_LIBRARY_PATH`.
- tomlplusplus needs C++17 (used by the `toml++/toml.hpp` header); fast-cpp-csv-parser only needs C++11 (declared in `third-part/CMakeLists.txt:32`).

## VS Code

`.vscode/` provides tasks (`cmake-configure` uses `--fresh`, `cmake-build` is default, `run` is default test task) and a GDB launch config that auto-runs `cmake-build` and sets `cwd` to `${workspaceFolder}`. F5 works out of the box.

## Branch Workflow

- `main` is the release branch.
- `dev` is the integration branch. PRs merge `dev` into `main`.
- Work on feature branches off `dev`.

## Testing

No test framework is set up. Verify with the binary:

```sh
./build/src/snippet_executable
```

It logs every key in `config/config.toml` and parses `[data] fsv_test_file_in` (default `data/fast-cpp-csv-parser/in/ram.csv`). The VS Code `run` task (`tasks.json:47-56`) is the default test task.
