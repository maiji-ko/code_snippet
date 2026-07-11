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
    if (!g_logger.init()) {
        spdlog::error("Failed to initialize default logger (cwd={})",
                      fs::current_path().string());
    }

    fs::path config_path = find_config_file();
    if (config_path.empty() || !g_config.load(config_path.string())) {
        spdlog::error("Failed to load config file: {}", config_path.string());
        return false;
    }

    const auto log_level = g_config.get_string("log", "level").value_or("info");
    g_logger.set_level(log_level);

    spdlog::info("Config loaded from: {}", config_path.string());

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
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%L] %v");

    if (!init()) {
        return 1;
    }

    run();

    return 0;
}
