# code_snippet

一个 C++17 示例项目，演示通过 CMake `FetchContent` 集成 [tomlplusplus](https://github.com/marzer/tomlplusplus)（TOML 配置解析）、[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)（CSV 解析）和 [spdlog](https://github.com/gabime/spdlog)（日志）。

## 构建与运行

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/src/snippet_executable
```

需要 CMake >= 3.20.1 和 C++17 编译器。依赖项在配置阶段自动下载，无需手动操作。

## 调试

在 VS Code 中打开项目，按 **F5** 即可开始调试。`.vscode/` 目录提供：

- `tasks.json` — 编译任务（cmake-configure、cmake-build、run）
- `launch.json` — GDB 调试配置，启动前自动编译
- `c_cpp_properties.json` — IntelliSense 配置（C++17、GCC、包含路径）

## 项目结构

| 路径 | 说明 |
|---|---|
| `src/main.cpp` | 入口 — 加载配置、通过 spdlog 记录日志、解析 CSV |
| `src/tomlplusplus/` | `ConfigParser` 封装类，基于 tomlplusplus |
| `src/fast-cpp-csv-parser/` | CSV 解析器的本地封装 |
| `config/config.toml` | 应用程序配置 |
| `data/fast-cpp-csv-parser/in/ram.csv` | CSV 测试数据 |
| `third-part/CMakeLists.txt` | FetchContent 声明 |
| `.vscode/` | VS Code 编译与调试配置 |

## 依赖

- **[tomlplusplus](https://github.com/marzer/tomlplusplus)** (MIT) — 仅头文件的 TOML 解析库
- **[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)** (BSD-3-Clause) — 仅头文件的 CSV 解析库
- **[spdlog](https://github.com/gabime/spdlog)** (MIT) — 仅头文件的日志库；以共享库方式构建（`SPDLOG_BUILD_SHARED=ON`），通过 `spdlog::spdlog` 链接。可执行文件的 `BUILD_RPATH` 已指向 `build/_deps/spdlog-build`，因此直接运行 `./build/src/snippet_executable` 无需设置 `LD_LIBRARY_PATH`。

三者均由 CMake 在配置阶段自动下载，无需手动安装。

## 配置

编辑 `config/config.toml` 修改项目路径和数据文件位置：

```toml
[project]
name = "code_snippet"
version = "1.0.0"

[data]
fsv_test_file_in = "data/fast-cpp-csv-parser/in/ram.csv"
fsv_test_file_out = "data/fast-cpp-csv-parser/in/ram_out.csv"
```

CSV 路径在运行时通过 `project_root` 和 `fsv_test_file_in` 拼接得到。

## 分支工作流

- `main` — 发布分支
- `dev` — 集成分支；PR 合并 `dev` 到 `main`
- 功能分支从 `dev` 创建
