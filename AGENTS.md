# AGENTS.md

## Build & Run

```sh
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

Requires CMake >= 3.20.1 and a C++17 compiler.

## Project Structure

| Path | Role |
|---|---|
| `src/main.cpp` | Entrypoint — calls `sample_fsv()` |
| `src/fast-cpp-csv-parser/` | Local wrapper around the CSV parser |
| `third-part/fast-cpp-csv-parser/` | Auto-downloaded by CMake `FetchContent` — [ben-strasser/fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) (header-only, BSD-3-Clause) |
| `third-part/tomlplusplus/` | Auto-downloaded by CMake `FetchContent` — [marzer/tomlplusplus](https://github.com/marzer/tomlplusplus) (header-only, MIT) |
| `data/fast-cpp-csv-parser/in/ram.csv` | Test data |

## Quirks

- **Hardcoded absolute path** in `src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp:19`. Update it to match your machine before running.

## Branch Workflow

- `main` is the release branch.
- `dev` is the integration branch. PRs merge `dev` into `main`.
- Work on feature branches off `dev`.

## Testing

No test framework is set up. Run the binary and check stdout/stderr manually.
