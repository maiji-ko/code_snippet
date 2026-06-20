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
    fs::path configPath = fs::current_path() / "config" / "config.toml";
    if (fs::exists(configPath)) {
        return configPath;
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
    if (auto csv_path = g_config.get_abs_path("data", "fsv_test_file_in")) {
        sample_fsv(csv_path->string());
    } else {
        std::cerr << "Failed to resolve CSV path from config\n";
    }
}

int main()
{
    if (!init()) {
        return 1;
    }

    run();

    return 0;
}
