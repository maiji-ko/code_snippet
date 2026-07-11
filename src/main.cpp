#include <filesystem>
#include <vector>

#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "fast-cpp-csv-parser/fast-cpp-csv-parser.h"
#include "tomlplusplus/tomlplusplus.h"
#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

ConfigParser g_config;
Logger       g_logger;

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
        spdlog::error("Failed to load config file: {}", config_path.string());
        return false;
    }

    const auto log_dir   = g_config.get_string("log", "dir").value_or("log");
    const auto log_file  = g_config.get_string("log", "file").value_or("spdlog");
    const auto log_level = g_config.get_string("log", "level").value_or("info");

    if (!g_logger.init(fs::current_path(), log_dir, log_file, log_level)) {
        spdlog::error("Failed to initialize logger ({} dir={} file={} level={})",
                      fs::current_path().string(), log_dir, log_file, log_level);
    }

    spdlog::info("Config loaded from: {}", config_path.string());

    if (auto abs_path = g_config.get_abs_path("data", "fsv_test_file_in")) {
        spdlog::info("fsv_test_file_in absolute path: {}", abs_path->string());
    }

    for (const auto& section : g_config.get_sections()) {
        spdlog::info("[{}]", section);
        for (const auto& key : g_config.get_keys(section)) {
            if (auto value = g_config.get_string(section, key)) {
                spdlog::info("  {} = {}", key, *value);
            }
        }
    }

    return true;
}

void run() {
    if (auto csv_path = g_config.get_abs_path("data", "fsv_test_file_in")) {
        sample_fsv(csv_path->string());
    } else {
        spdlog::error("Failed to resolve CSV path from config");
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
