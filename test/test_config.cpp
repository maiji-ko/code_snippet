#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <cstdio>
#include <filesystem>
#include <fstream>

#include "tomlplusplus/tomlplusplus.h"

namespace fs = std::filesystem;
using namespace Catch::Matchers;

struct TomlFixture {
    fs::path path;

    TomlFixture()
    {
        path = fs::temp_directory_path() / "test_config.toml";
        std::ofstream(path) <<
            "[project]\n"
            "name = \"test_project\"\n"
            "version = \"2.0.0\"\n"
            "\n"
            "[data]\n"
            "input = \"data/in.csv\"\n"
            "\n"
            "[log]\n"
            "level = \"debug\"\n";
    }

    ~TomlFixture()
    {
        std::error_code ec;
        fs::remove(path, ec);
    }
};

TEST_CASE_METHOD(TomlFixture, "ConfigParser loads a valid TOML file", "[config]")
{
    ConfigParser cfg;
    REQUIRE(cfg.load(path.string()));
    CHECK(cfg.is_loaded());
}

TEST_CASE_METHOD(TomlFixture, "ConfigParser get_string returns correct values", "[config]")
{
    ConfigParser cfg;
    REQUIRE(cfg.load(path.string()));

    auto name = cfg.get_string("project", "name");
    REQUIRE(name.has_value());
    CHECK(*name == "test_project");

    auto ver = cfg.get_string("project", "version");
    REQUIRE(ver.has_value());
    CHECK(*ver == "2.0.0");
}

TEST_CASE_METHOD(TomlFixture, "ConfigParser get_string returns nullopt for missing key", "[config]")
{
    ConfigParser cfg;
    REQUIRE(cfg.load(path.string()));

    auto val = cfg.get_string("project", "nonexistent");
    CHECK_FALSE(val.has_value());
}

TEST_CASE_METHOD(TomlFixture, "ConfigParser get_string returns nullopt for missing section", "[config]")
{
    ConfigParser cfg;
    REQUIRE(cfg.load(path.string()));

    auto val = cfg.get_string("nosuch", "key");
    CHECK_FALSE(val.has_value());
}

TEST_CASE_METHOD(TomlFixture, "ConfigParser get_sections lists all sections", "[config]")
{
    ConfigParser cfg;
    REQUIRE(cfg.load(path.string()));

    auto sections = cfg.get_sections();
    REQUIRE(sections.size() == 3);
    CHECK_THAT(sections, Catch::Matchers::UnorderedEquals(
        std::vector<std::string>{"project", "data", "log"}));
}

TEST_CASE_METHOD(TomlFixture, "ConfigParser returns false for nonexistent file", "[config]")
{
    ConfigParser cfg;
    CHECK_FALSE(cfg.load("/nonexistent/file.toml"));
    CHECK_FALSE(cfg.is_loaded());
}

TEST_CASE_METHOD(TomlFixture, "ConfigParser get_abs_path resolves relative path", "[config]")
{
    ConfigParser cfg;
    REQUIRE(cfg.load(path.string()));

    auto abs_path = cfg.get_abs_path("data", "input");
    REQUIRE(abs_path.has_value());
    CHECK(abs_path->is_absolute());
    CHECK(abs_path->filename() == "in.csv");
}
