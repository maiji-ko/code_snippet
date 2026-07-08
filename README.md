# code_snippet

一个 C++17 示例项目，演示通过 **git 子模块**集成 [tomlplusplus](https://github.com/marzer/tomlplusplus)（TOML 配置解析）、[fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)（CSV 解析）和 [spdlog](https://github.com/gabime/spdlog)（日志）。

## 构建与运行

```sh
git submodule update --init --recursive
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/src/snippet_executable
```

需要 CMake >= 3.20.1 和 C++17 编译器。首次克隆后需初始化子模块（需要联网）。

> 运行可执行文件时请保持在仓库根目录——`src/main.cpp:18` 通过 `fs::current_path()` 解析 `config/config.toml` 的路径。

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
| `src/CMakeLists.txt` | 显式列出封装源文件（无 glob） |
| `third-part/` | 第三方依赖（git 子模块） |
| `third-part/CMakeLists.txt` | 通过 `add_subdirectory` 引入子模块；定义 `fast_csv` 接口库 |
| `config/config.toml` | 应用程序配置 |
| `data/fast-cpp-csv-parser/in/ram.csv` | CSV 测试数据 |

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