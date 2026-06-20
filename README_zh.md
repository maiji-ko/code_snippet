# code_snippet

一个 C++17 示例项目，演示通过 CMake `FetchContent` 集成 [tomlplusplus](https://github.com/marzer/tomlplusplus)（TOML 配置解析）和 [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)（CSV 解析）。

## 构建

```sh
cmake -S . -B build
cmake --build build
./build/src/snippet_executable
```

需要 CMake >= 3.20.1 和 C++17 编译器。依赖项在配置阶段自动下载，无需手动操作。

## 项目结构

| 路径 | 说明 |
|---|---|
| `src/main.cpp` | 入口 — 加载配置、解析 CSV |
| `src/tomlplusplus/` | `ConfigParser` 封装类，基于 tomlplusplus |
| `src/fast-cpp-csv-parser/` | CSV 解析器的本地封装 |
| `config/config.toml` | 应用程序配置 |
| `data/fast-cpp-csv-parser/in/ram.csv` | CSV 测试数据 |
| `third-part/CMakeLists.txt` | FetchContent 声明 |

## 依赖

- **[tomlplusplus](https://github.com/marzer/tomlplusplus)** (MIT) — 仅头文件的 TOML 解析库
- **[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)** (BSD-3-Clause) — 仅头文件的 CSV 解析库

两者均由 CMake 在配置阶段自动下载，无需手动安装。

## 配置

编辑 `config/config.toml` 修改项目路径和数据文件位置：

```toml
[project]
name = "code_snippet"
version = "1.0.0"

[paths]
project_root = "/path/to/project"

[data]
fsv_test_file_in = "data/fast-cpp-csv-parser/in/ram.csv"
```

## 已知问题

- `src/fast-cpp-csv-parser/fast-cpp-csv-parser.cpp:19` 包含硬编码的绝对路径，项目位置不同时需要更新。

## 分支工作流

- `main` — 发布分支
- `dev` — 集成分支；PR 合并 `dev` 到 `main`
- 功能分支从 `dev` 创建
