#include "spdlog.h"

#include <iostream>
#include <utility>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace {

spdlog::level::level_enum parse_level(const std::string& level) {
    if (level == "trace")    return spdlog::level::trace;
    if (level == "debug")    return spdlog::level::debug;
    if (level == "warn" ||
        level == "warning")  return spdlog::level::warn;
    if (level == "error" ||
        level == "err")      return spdlog::level::err;
    if (level == "critical") return spdlog::level::critical;
    if (level == "off")      return spdlog::level::off;
    return spdlog::level::info;
}

}  // namespace

Logger::~Logger() {
    shutdown();
}

bool Logger::init(const fs::path& project_root,
                  const std::string& log_dir,
                  const std::string& base_filename,
                  const std::string& level) {
    fs::path dir = (project_root / log_dir).make_preferred();

    std::error_code ec;
    fs::create_directories(dir, ec);
    if (ec) {
        std::cerr << "Failed to create log dir " << dir << ": " << ec.message() << "\n";
        return false;
    }

    const fs::path file_pattern = (dir / (base_filename + ".log")).make_preferred();

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(parse_level(level));

    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
        file_pattern.string(), 0, 0);
    file_sink->set_level(parse_level(level));

    auto logger = std::make_shared<spdlog::logger>("default",
                                                   spdlog::sinks_init_list{console_sink, file_sink});
    logger->set_level(parse_level(level));
    logger->flush_on(spdlog::level::warn);

    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    logger_ = std::move(logger);
    initialized_ = true;
    return true;
}

void Logger::shutdown() {
    if (!initialized_) return;
    if (logger_) {
        logger_->flush();
    }
    spdlog::drop("default");
    spdlog::set_default_logger(std::make_shared<spdlog::logger>("default", spdlog::sinks_init_list{
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
    }));
    initialized_ = false;
}
