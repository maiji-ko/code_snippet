# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

This project uses CMake (minimum 3.30.5).

```bash
# Configure and build
cmake -B build -S .
cmake --build build

# Run the executable
./build/src/snippet_executable.exe
```

## Architecture

This is a C++ code snippet repository for testing and experimenting with third-party libraries.

### Directory Structure
- `src/` - Main application code with sample implementations
- `third-part/` - Vendored header-only libraries
- `data/` - Sample input files (CSV, etc.)

### Third-Party Libraries

**fast-cpp-csv-parser** (`third-part/fast-cpp-csv-parser/`)
- Header-only CSV parser requiring C++11
- Main header: `csv.h`
- Usage: `io::CSVReader<N>` where N is column count
- Example in `src/fast-cpp-csv-parser/`

**tomlplusplus** (`third-part/tomlplusplus-3.4.0/`)
- Header-only TOML parser requiring C++17
- Main header: `toml++/toml.hpp`
- Wrapper class `ConfigParser` in `src/tomlplusplus/`

### CMake Configuration

- Executable target: `snippet_executable`
- Libraries: `fast_csv` (interface), `tomlplusplus_tomlplusplus` (interface)
- C++17 required (for tomlplusplus compatibility)

When adding new source files, update `src/CMakeLists.txt`.
