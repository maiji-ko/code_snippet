#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

class Logger {
public:
    Logger() = default;
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Initialize the default logger with both stdout + daily rotating file sinks.
    //   project_root  : base directory the log dir is resolved against (default = cwd)
    //   log_dir       : subdirectory under project_root that holds the log files (default "log")
    //   base_filename : basename for rotating files (default "spdlog")
    //   level         : spdlog level name (trace|debug|info|warn|error|critical|off, default "info")
    bool init(const fs::path& project_root = fs::current_path(),
              const std::string& log_dir = "log",
              const std::string& base_filename = "spdlog",
              const std::string& level = "info");

    bool set_level(const std::string& level);

    bool is_initialized() const { return initialized_; }

    // Returns the configured default logger (or spdlog's default if init was skipped).
    std::shared_ptr<spdlog::logger> get() const { return logger_; }

    // Drop the default logger and flush on shutdown.
    void shutdown();

private:
    bool initialized_ = false;
    std::shared_ptr<spdlog::logger> logger_;
};
