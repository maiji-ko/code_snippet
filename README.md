# code_snippet

[简体中文](./README.md) | [English](./readme/README_en.md)

<p align="center">
  <img src="./assets/icon.svg" alt="code_snippet logo" width="160">
</p>

一个 C++17 示例项目，演示通过 **git 子模块**集成 [tomlplusplus](https://github.com/marzer/tomlplusplus)（TOML 配置解析）、[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)（CSV 解析）和 [spdlog](https://github.com/gabime/spdlog)（日志）。

项目为每个第三方库提供一层本地轻量封装（`Logger`、`ConfigParser`、CSV 包装），源代码镜像组织成 `src/<lib>/<lib>.{h,cpp}`，与 `third-part/` 下的子模块一一对应。

## 构建与运行

```sh
git submodule update --init --recursive
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/src/snippet_executable
```

需要 CMake >= 3.20.1 和 C++17 编译器。首次克隆后需初始化子模块（需要联网）。

> 运行可执行文件时请保持在仓库根目录——`src/main.cpp:18` 通过 `fs::current_path()` 解析 `config/config.toml` 的路径。

运行后，控制台与 `log/<file>_YYYY-MM-DD.log` 文件会同时输出日志（默认 basename 为 `spdlog`，可通过配置修改）。

## 调试

在 VS Code 中打开项目，按 **F5** 即可开始调试。`.vscode/` 目录提供：

- `tasks.json` — 编译任务（cmake-configure、cmake-build、run）
- `launch.json` — GDB 调试配置，启动前自动编译
- `c_cpp_properties.json` — IntelliSense 配置（C++17、GCC、包含路径）

## 项目结构

| 路径 | 说明 |
|---|---|
| `src/main.cpp` | 入口 — 加载配置、初始化日志器、解析 CSV |
| `src/tomlplusplus/` | `ConfigParser` 封装类，基于 tomlplusplus |
| `src/fast-cpp-csv-parser/` | CSV 解析器的本地封装 |
| `src/spdlog/` | `Logger` 封装类，基于 spdlog，自动落盘到 `log/` 目录 |
| `src/CMakeLists.txt` | 显式列出封装源文件（无 glob） |
| `third-part/` | 第三方依赖（git 子模块） |
| `third-part/CMakeLists.txt` | 通过 `add_subdirectory` 引入子模块；定义 `fast_csv` 接口库 |
| `config/config.toml` | 应用程序配置（项目元信息、数据路径、日志设置） |
| `data/fast-cpp-csv-parser/in/ram.csv` | CSV 测试数据 |
| `log/` | 运行时日志目录（首次运行自动创建） |

## 依赖

| 依赖 | 协议 | 角色 |
|---|---|---|
| [tomlplusplus](https://github.com/marzer/tomlplusplus) `v3.4.0` | MIT | 仅头文件的 TOML 解析库 |
| [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser) (master HEAD) | BSD-3-Clause | 仅头文件的 CSV 解析库 |
| [spdlog](https://github.com/gabime/spdlog) `v1.17.0` | MIT | 仅头文件的日志库；以共享库方式构建（`SPDLOG_BUILD_SHARED=ON`），通过 `spdlog::spdlog` 链接 |

三者均通过 `.gitmodules` 注册为 git 子模块，在 `third-part/` 下以 `add_subdirectory` 的方式引入。spdlog 的构建产物位于 `build/third-part/spdlog/`，可执行文件的 `BUILD_RPATH` 已指向该目录，因此直接运行 `./build/src/snippet_executable` 无需设置 `LD_LIBRARY_PATH`。

## 子模块固定版本

| 提交 SHA | 仓库 | 标签 / 分支 |
|---|---|---|
| `30172438cee64926dc41fdd9c11fb3ba5b2ba9de` | marzer/tomlplusplus | `v3.4.0` |
| `574a9fe4d323ba63416877a4a5fe59088d37aa34` | ben-strasser/fast-cpp-csv-parser | master HEAD |
| `79524ddd08a4ec981b7fea76afd08ee05f83755d` | gabime/spdlog | `v1.17.0` |

升级某个依赖：编辑 `.gitmodules`（URL），`git submodule sync`，然后在子模块内 `git fetch --depth=1 origin <ref>`、`git checkout <ref>`，最后在父仓库提交新的 gitlink SHA。

## 配置

编辑 `config/config.toml` 修改项目路径、数据文件位置和日志设置：

```toml
[project]
name = "code_snippet"
version = "1.0.0"

[data]
fsv_test_file_in = "data/fast-cpp-csv-parser/in/ram.csv"
fsv_test_file_out = "data/fast-cpp-csv-parser/out/ram_out.csv"

[log]
dir  = "log"        # 相对于项目根目录
file = "spdlog"     # 日志文件 basename（按天切分，自动追加 _YYYY-MM-DD.log）
level = "info"      # trace|debug|info|warn|error|critical|off
```

CSV 路径在运行时通过 `project_root` 和 `fsv_test_file_in` 拼接得到。日志器在 `Logger::init()` 中读取 `[log]` 段并注册为 spdlog 的 default logger，因此 `spdlog::*` 系列调用会同时输出到控制台与日志文件。

## 添加新代码

- 新 `.cpp` 文件须显式加入 `src/CMakeLists.txt:3-7`（不使用 glob）。
- 新第三方依赖放到 `third-part/CMakeLists.txt`，通过 `add_subdirectory` 引入后，在 `src/CMakeLists.txt:9-14` 中链接；若为共享库还需同步 `src/CMakeLists.txt:16-19` 的 `BUILD_RPATH`。
- 按现有模式新增对某个第三方库的本地封装：建立 `src/<lib>/<lib>.{h,cpp}`，暴露一个简单的类（如 `ConfigParser`、`Logger`），并在 `main.cpp` 中调用其初始化方法。

## 分支工作流

- `main` — 发布分支
- `dev` — 集成分支；PR 合并 `dev` 到 `main`
- 功能分支从 `dev` 创建

## 文档索引

| 文件 | 说明 |
|---|---|
| `README.md` | 本文件（中文） |
| `readme/README_en.md` | 英文版文档 |
| `AGENTS.md` | AI 代理工作守则（构建、运行、目录约定、踩坑指南） |
