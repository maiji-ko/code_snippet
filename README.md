# code_snippet

A C++17 project demonstrating [tomlplusplus](https://github.com/marzer/tomlplusplus) (TOML config parsing) and [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) (CSV parsing) integrated via CMake `FetchContent`.

## Build

```sh
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

Requires CMake >= 3.20.1 and a C++17 compiler. Dependencies are auto-downloaded at configure time.

## Project Structure

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — loads config, parses CSV |
| `src/tomlplusplus/` | `ConfigParser` wrapper around tomlplusplus |
| `src/fast-cpp-csv-parser/` | Local wrapper around the CSV parser |
| `config/config.toml` | Application configuration |
| `data/fast-cpp-csv-parser/in/ram.csv` | Sample CSV test data |
| `third-part/CMakeLists.txt` | FetchContent declarations |

## Dependencies

- **[tomlplusplus](https://github.com/marzer/tomlplusplus)** (MIT) — header-only TOML parser
- **[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)** (BSD-3-Clause) — header-only CSV parser

Both are downloaded automatically by CMake during configuration; no manual setup needed.

## Configuration

Edit `config/config.toml` to change project paths and data file locations:

```toml
[project]
name = "code_snippet"
version = "1.0.0"

[paths]
project_root = "/path/to/project"

[data]
fsv_test_file_in = "data/fast-cpp-csv-parser/in/ram.csv"
```

## Known Issues

- `src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp:19` contains a hardcoded absolute Windows path. Update it to the correct path on your machine before running.
- `config/config.toml` also contains a Windows-specific `project_root` path — adjust it for your environment.

## Branch Workflow

- `main` — release branch
- `dev` — integration branch; PRs merge `dev` into `main`
- Feature branches off `dev`
