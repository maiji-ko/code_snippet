# code_snippet

A C++17 project demonstrating [tomlplusplus](https://github.com/marzer/tomlplusplus) (TOML config parsing), [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) (CSV parsing), and [spdlog](https://github.com/gabime/spdlog) (logging) integrated via CMake `FetchContent`.

## Build & Run

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/src/snippet_executable
```

Requires CMake >= 3.20.1 and a C++17 compiler. Dependencies are auto-downloaded at configure time.

## Debug

Open the project in VS Code and press **F5**. The `.vscode/` directory provides:

- `tasks.json` — build tasks (cmake-configure, cmake-build, run)
- `launch.json` — GDB debug configuration, auto-builds before launch
- `c_cpp_properties.json` — IntelliSense settings (C++17, GCC, include paths)

## Project Structure

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — loads config, logs via spdlog, parses CSV |
| `src/tomlplusplus/` | `ConfigParser` wrapper around tomlplusplus |
| `src/fast-cpp-csv-parser/` | Local wrapper around the CSV parser |
| `config/config.toml` | Application configuration |
| `data/fast-cpp-csv-parser/in/ram.csv` | Sample CSV test data |
| `third-part/CMakeLists.txt` | FetchContent declarations |
| `.vscode/` | VS Code build & debug configuration |

## Dependencies

- **[tomlplusplus](https://github.com/marzer/tomlplusplus)** (MIT) — header-only TOML parser
- **[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)** (BSD-3-Clause) — header-only CSV parser
- **[spdlog](https://github.com/gabime/spdlog)** (MIT) — header-only logging library; built as a shared library (`SPDLOG_BUILD_SHARED=ON`) and linked via `spdlog::spdlog`. The executable's `BUILD_RPATH` points at `build/_deps/spdlog-build`, so `./build/src/snippet_executable` runs without `LD_LIBRARY_PATH`.

All three are downloaded automatically by CMake during configuration; no manual setup needed.

## Configuration

Edit `config/config.toml` to change project paths and data file locations:

```toml
[project]
name = "code_snippet"
version = "1.0.0"

[data]
fsv_test_file_in = "data/fast-cpp-csv-parser/in/ram.csv"
fsv_test_file_out = "data/fast-cpp-csv-parser/in/ram_out.csv"
```

The CSV path is resolved at runtime by combining the `project_root` from config with the relative `fsv_test_file_in` path.

## Branch Workflow

- `main` — release branch
- `dev` — integration branch; PRs merge `dev` into `main`
- Feature branches off `dev`
