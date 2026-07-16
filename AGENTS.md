# AGENTS.md

## Build & Run

```sh
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

- Requires CMake >= 3.20.1 (`CMakeLists.txt:1`) and a C++17 compiler.
- Third-party deps (tomlplusplus, fast-cpp-csv-parser, spdlog, Catch2) live in `third-part/` as **git submodules** (`.gitmodules`). First-time setup needs network access to clone them.
- Root `CMakeLists.txt:3` hard-sets `CMAKE_CXX_COMPILER=clang++`. Edit or unset there to use a different compiler.
- Default `CMAKE_BUILD_TYPE=Debug` (`CMakeLists.txt:7-9`).

### Gotchas

- **Run the binary from the repo root.** `src/main.cpp:18` resolves `config/config.toml` via `fs::current_path()`, and `ConfigParser::load` (`src/tomlplusplus/tomlplusplus.cpp:12`) records `fs::current_path()` as `m_projectRoot` for `get_abs_path()`. Running from `build/` (or anywhere else) silently breaks both config lookup and path resolution.
- **Generator mismatch.** If configure fails with `generator : Ninja Does not match the generator used previously: Unix Makefiles`, run `rm -rf build` (or use a fresh `-B build-<name>`) and reconfigure.
- **spdlog is built as a shared library** (`SPDLOG_BUILD_SHARED=ON` in `third-part/CMakeLists.txt:18`). Build artifacts land in `build/third-part/spdlog/`, and `src/CMakeLists.txt:16-19` sets `BUILD_RPATH` to that dir so `./build/src/snippet_executable` runs without `LD_LIBRARY_PATH`. Update `BUILD_RPATH` when adding another shared dep.
- **`config.toml` references an unwritten output path.** `[data] fsv_test_file_out` points to `data/fast-cpp-csv-parser/out/ram_out.csv`, but the `out/` directory is not created. The current `sample_fsv()` only reads, so a build is fine, but agents writing output must `mkdir -p` the dir first.

## Submodules

| Pinned commit | Repo | Tag / Branch |
|---|---|---|
| `30172438cee64926dc41fdd9c11fb3ba5b2ba9de` | marzer/tomlplusplus | `v3.4.0` |
| `574a9fe4d323ba63416877a4a5fe59088d37aa34` | ben-strasser/fast-cpp-csv-parser | master HEAD |
| `79524ddd08a4ec981b7fea76afd08ee05f83755d` | gabime/spdlog | `v1.17.0` |
| `191fa38c9b1596cd2576ab531d4ab4d5e8e05190` | catchorg/Catch2 | `v3.15.2` |

To bump a dep: edit `.gitmodules` (URL), run `git submodule sync`, then inside the submodule `git fetch --depth=1 origin <ref>` and `git checkout <ref>`, then commit the parent's new gitlink SHA.

## Project Layout

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — loads `config/config.toml`, logs every key via spdlog, calls `sample_fsv()` |
| `src/tomlplusplus/` | `ConfigParser` wrapper around `<toml++/toml.hpp>` (`get_string`, `get_int`, `get_bool`, `get_abs_path`, ...) |
| `src/fast-cpp-csv-parser/` | `int sample_fsv(const std::string& csv_path)` wrapper around `csv.h` |
| `src/CMakeLists.txt` | Lists wrapper `.cpp` files explicitly (no globbing) |
| `third-part/CMakeLists.txt` | `add_subdirectory` for tomlplusplus, spdlog & Catch2; exposes `fast_csv` and `tomlplusplus::tomlplusplus` targets, plus `spdlog::spdlog` and `Catch2::Catch2WithMain` |
| `config/config.toml` | Runtime config — `[project]` (name/version) and `[data]` (CSV paths) |
| `data/fast-cpp-csv-parser/in/ram.csv` | Sample CSV (3 columns: `vendor`, `size`, `speed`) |

## Adding Code

- New `.cpp` files must be added to the explicit list in `src/CMakeLists.txt:3-7` (no globbing).
- New third-party deps go in `third-part/CMakeLists.txt` via `add_subdirectory`, then are linked in `src/CMakeLists.txt:9-14`. Shared deps also need the `BUILD_RPATH` update at `src/CMakeLists.txt:16-19`.
- **Whenever a module is added, removed, or a dep is bumped, keep docs in sync in the same change:**
  - `LICENSE` — third-party block (项目元信息 + 每个子模块的 pin / 协议 / 指向 `third-part/<lib>/LICENSE`).
  - `README.md` + `readme/README_en.md` — Project Layout 表、依赖表、配置示例、添加代码约定；中英两边都要改.
  - `AGENTS.md` — Submodules 表、Project Layout 表、Build & Run 或 Adding Code 段里的相关描述.

## VS Code

`.vscode/tasks.json` provides `cmake-configure` (uses `--fresh`), `cmake-build` (default), `cmake-clean`, and `run` (default test task). `.vscode/launch.json` is a GDB config that auto-runs `cmake-build` and sets `cwd` to `${workspaceFolder}` — F5 works out of the box.

## Testing

Tests use **Catch2 v3.15.2** (`Catch2::Catch2WithMain`). Build and run with CTest:

```sh
cmake -S . -B build
cmake --build build
cd build && ctest --output-on-failure
```

Or run test binaries directly:

```sh
./build/test/test_csv
./build/test/test_config
```

The test targets are defined in `test/CMakeLists.txt` and cover CSV parsing (`test_csv`) and config parsing (`test_config`).