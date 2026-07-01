# AGENTS.md

## Build & Run

```sh
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

- Requires CMake >= 3.20.1 (root `CMakeLists.txt:1`) and a C++17 compiler. Default `CMAKE_BUILD_TYPE` is `Debug` (root `CMakeLists.txt:5-7`). No `CMAKE_CXX_STANDARD` is set anywhere — the project relies on the compiler default being C++17+ (true for GCC 11+, Clang 14+, MSVC 19.30+).
- Third-party deps (tomlplusplus, fast-cpp-csv-parser, spdlog) are auto-downloaded by CMake `FetchContent` (`third-part/CMakeLists.txt:1-44`). The first configure needs network access.
- **Generator gotcha:** CMake refuses to switch generators in-place. If you see `Error: generator : Ninja Does not match the generator used previously: Unix Makefiles`, run `rm -rf build` (or pick a fresh `-B build-<name>`) and reconfigure. The VS Code `cmake-configure` task already uses `--fresh` to avoid this.
- **Run the binary from the repo root.** `src/main.cpp:17-24` resolves the config path as `fs::current_path() / "config" / "config.toml"`, and `ConfigParser::load` (`src/tomlplusplus/tomlplusplus.cpp:12`) sets `m_projectRoot` from `fs::current_path()`. Running from `build/` or any other directory makes config lookup and `get_abs_path()` return wrong paths.
- spdlog is built as a shared library (`SPDLOG_BUILD_SHARED=ON` in `third-part/CMakeLists.txt:42`). `src/CMakeLists.txt:16-19` sets `BUILD_RPATH` to `_deps/spdlog-build` so `./build/src/snippet_executable` runs without `LD_LIBRARY_PATH` — keep this when editing `src/CMakeLists.txt`.

## Project Structure

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — finds `config/config.toml`, logs config keys, calls `sample_fsv()` |
| `src/tomlplusplus/` | Wrapper: `ConfigParser` (`get_string`, `get_int`, `get_abs_path`, ...) around `<toml++/toml.hpp>` |
| `src/fast-cpp-csv-parser/` | Wrapper: `int sample_fsv(const std::string& csv_path)` around `csv.h` |
| `src/CMakeLists.txt` | Executable target `snippet_executable`; lists wrapper `.cpp` files explicitly, not globbed |
| `third-part/CMakeLists.txt` | `FetchContent` declarations; exposes `fast_csv` (INTERFACE, `cxx_std_11`) |
| `config/config.toml` | Runtime config; section `[project]` (name/version) and `[data]` (CSV paths) |
| `data/fast-cpp-csv-parser/in/ram.csv` | Sample CSV (3 columns: `vendor`, `size`, `speed`) |

## Adding Code

- Add new source files to the `add_executable(...)` list at `src/CMakeLists.txt:3-7` (sources are listed explicitly, not globbed).
- Add new third-party deps via `FetchContent` in `third-part/CMakeLists.txt`, then link the target in `src/CMakeLists.txt:9-14`. For shared libraries, also update `BUILD_RPATH` (`src/CMakeLists.txt:16-19`).

## VS Code

`.vscode/tasks.json` provides `cmake-configure` (uses `--fresh`), `cmake-build` (default build), `cmake-clean`, and `run` (default test task). `.vscode/launch.json` is a GDB config that auto-runs `cmake-build` and sets `cwd` to `${workspaceFolder}`. F5 works out of the box.

## Branch Workflow

- `main` is the release branch.
- `dev` is the integration branch. PRs merge `dev` into `main`.
- Work on feature branches off `dev`.

## Testing

No test framework, no CI (no `.github/`). Verify with the binary from the repo root:

```sh
./build/src/snippet_executable
```

It logs every key in `config/config.toml` and parses `[data] fsv_test_file_in` (default `data/fast-cpp-csv-parser/in/ram.csv`). The VS Code `run` task (`.vscode/tasks.json:46-56`) wraps this.
