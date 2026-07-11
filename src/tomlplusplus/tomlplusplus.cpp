#include "tomlplusplus.h"

#include <spdlog/spdlog.h>

#include <string>

ConfigParser::ConfigParser(const std::string& config_path) {
    load(config_path);
}

bool ConfigParser::load(const std::string& config_path) {
    spdlog::info("{} loading config: {}", kTomlTag, config_path);
    try {
        table_ = toml::parse_file(config_path);
        loaded_ = true;
        m_projectRoot = fs::current_path();

        size_t entry_count = 0;
        size_t section_count = 0;
        for (auto& [key, value] : table_) {
            ++entry_count;
            if (value.is_table()) {
                ++section_count;
            }
        }
        spdlog::info("{} parsed OK ({} entries, {} section(s), project_root={})",
                     kTomlTag, entry_count, section_count, m_projectRoot.string());

        if (auto name = get_string("project", "name")) {
            spdlog::info("{} project.name = {}", kTomlTag, *name);
        }
        if (auto ver = get_string("project", "version")) {
            spdlog::info("{} project.version = {}", kTomlTag, *ver);
        }
        return true;
    } catch (const toml::parse_error& err) {
        spdlog::error("{} failed to parse '{}': {}",
                      kTomlTag, config_path, err.description());
        loaded_ = false;
        return false;
    }
}

std::vector<std::string> ConfigParser::get_sections() const {
    std::vector<std::string> sections;
    if (!loaded_) {
        spdlog::warn("{} get_sections() called before a successful load()", kTomlTag);
        return sections;
    }

    for (auto& [key, value] : table_) {
        if (value.is_table()) {
            sections.push_back(std::string(key.str()));
        }
    }
    spdlog::debug("{} get_sections() -> {} section(s)", kTomlTag, sections.size());
    return sections;
}

std::vector<std::string> ConfigParser::get_keys(const std::string& section) const {
    std::vector<std::string> keys;
    if (!loaded_) {
        spdlog::warn("{} get_keys('{}') called before a successful load()", kTomlTag, section);
        return keys;
    }

    auto node = table_.get(section);
    if (!node || !node->is_table()) {
        spdlog::warn("{} section [{}] not found or is not a table", kTomlTag, section);
        return keys;
    }

    auto& subtable = *node->as_table();
    for (auto& [key, value] : subtable) {
        keys.push_back(std::string(key.str()));
    }
    spdlog::debug("{} [{}] -> {} key(s)", kTomlTag, section, keys.size());
    return keys;
}

std::optional<fs::path> ConfigParser::get_abs_path(const std::string& section,
                                                   const std::string& key) const {
    auto relative_path = get_string(section, key);
    if (!relative_path) {
        spdlog::warn("{} [{}].{} not found or not a string", kTomlTag, section, key);
        return std::nullopt;
    }

    auto resolved = (m_projectRoot / *relative_path).make_preferred();
    spdlog::debug("{} [{}].{} -> {}", kTomlTag, section, key, resolved.string());
    return resolved;
}
