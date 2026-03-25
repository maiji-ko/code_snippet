#include "tomlplusplus.h"
#include <iostream>

ConfigParser::ConfigParser(const std::string& config_path) {
    load(config_path);
}

bool ConfigParser::load(const std::string& config_path) {
    try {
        table_ = toml::parse_file(config_path);
        loaded_ = true;
        return true;
    }
    catch (const toml::parse_error& err) {
        std::cerr << "Failed to parse config file: " << err << "\n";
        loaded_ = false;
        return false;
    }
}

std::vector<std::string> ConfigParser::get_sections() const {
    std::vector<std::string> sections;
    if (!loaded_) return sections;

    for (auto& [key, value] : table_) {
        if (value.is_table()) {
            sections.push_back(std::string(key.str()));
        }
    }
    return sections;
}

std::vector<std::string> ConfigParser::get_keys(const std::string& section) const {
    std::vector<std::string> keys;
    if (!loaded_) return keys;

    auto node = table_.get(section);
    if (!node || !node->is_table()) return keys;

    auto& subtable = *node->as_table();
    for (auto& [key, value] : subtable) {
        keys.push_back(std::string(key.str()));
    }
    return keys;
}
