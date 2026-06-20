#pragma once

#include <string>
#include <optional>
#include <vector>
#include <filesystem>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;

class ConfigParser {
public:
    ConfigParser() = default;
    explicit ConfigParser(const std::string& config_path);

    bool load(const std::string& config_path);
    bool is_loaded() const { return loaded_; }

    // Get all section names
    std::vector<std::string> get_sections() const;

    // Get all keys in a section
    std::vector<std::string> get_keys(const std::string& section) const;

    // Generic get by section and key
    template<typename T>
    std::optional<T> get(const std::string& section, const std::string& key) const {
        if (!loaded_) return std::nullopt;

        auto node = table_.at_path(section + "." + key);
        if (auto value = node.value<T>()) {
            return *value;
        }
        return std::nullopt;
    }

    // Convenience: get string
    std::optional<std::string> get_string(const std::string& section, const std::string& key) const {
        return get<std::string>(section, key);
    }

    // Convenience: get int
    std::optional<int64_t> get_int(const std::string& section, const std::string& key) const {
        return get<int64_t>(section, key);
    }

    // Convenience: get double
    std::optional<double> get_double(const std::string& section, const std::string& key) const {
        return get<double>(section, key);
    }

    // Convenience: get bool
    std::optional<bool> get_bool(const std::string& section, const std::string& key) const {
        return get<bool>(section, key);
    }

    // Get absolute path: project_root + relative path from config
    std::optional<fs::path> get_abs_path(const std::string& section, const std::string& key) const {
        auto project_root = get_string("paths", "project_root");
        auto relative_path = get_string(section, key);

        if (!project_root || !relative_path) {
            return std::nullopt;
        }

        return (fs::path(*project_root) / *relative_path).make_preferred();
    }

    // Get raw table
    const toml::table& raw_table() const { return table_; }

private:
    toml::table table_;
    bool loaded_ = false;
};
