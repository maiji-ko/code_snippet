#include <iostream>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "fast-cpp-csv-parser/fast-cpp-csv-parser.h"
#include "tomlplusplus/tomlplusplus.h"

namespace fs = std::filesystem;

ConfigParser g_config;

fs::path find_config_file() {
    // 尝试的路径列表
    std::vector<fs::path> candidates = {
        fs::current_path() / "config" / "config.toml",  // 当前工作目录
    };

    // 从可执行文件目录向上查找
    fs::path exe_path;
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    exe_path = fs::path(buffer).parent_path();
#else
    exe_path = fs::read_symlink("/proc/self/exe").parent_path();
#endif

    fs::path dir = exe_path;
    for (int i = 0; i < 5 && !dir.empty(); ++i) {
        candidates.push_back(dir / "config" / "config.toml");
        dir = dir.parent_path();
    }

    for (const auto& p : candidates) {
        if (fs::exists(p)) {
            return p;
        }
    }

    return {};
}

bool init() {
    fs::path config_path = find_config_file();

    if (config_path.empty() || !g_config.load(config_path.string())) {
        std::cerr << "Failed to load config file: " << config_path << "\n";
        return false;
    }

    std::cout << "Config loaded from: " << config_path << "\n\n";

    // Demo: get absolute path
    if (auto abs_path = g_config.get_abs_path("data", "fsv_test_file_in")) {
        std::cout << "fsv_test_file_in absolute path: " << *abs_path << "\n\n";
    }

    // Print all sections and config
    for (const auto& section : g_config.get_sections()) {
        std::cout << "[" << section << "]\n";
        for (const auto& key : g_config.get_keys(section)) {
            if (auto value = g_config.get_string(section, key)) {
                std::cout << "  " << key << " = " << *value << "\n";
            }
        }
    }

    return true;
}

void run() {
    sample_fsv();
}

int main()
{
    if (!init()) {
        return 1;
    }

    run();

    return 0;
}
