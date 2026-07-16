#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <cstdio>
#include <filesystem>
#include <fstream>

#include "fast-cpp-csv-parser/fast-cpp-csv-parser.h"

namespace fs = std::filesystem;

TEST_CASE("sample_fsv returns 0 on valid CSV", "[csv]")
{
    auto tmp = fs::temp_directory_path() / "test_ram.csv";
    std::ofstream(tmp) << "vendor,size,speed\nVendorA,8,2400\nVendorB,16,3200\n";
    REQUIRE(fs::exists(tmp));

    int ret = sample_fsv(tmp.string());
    CHECK(ret == 0);

    fs::remove(tmp);
}

TEST_CASE("sample_fsv returns 1 on missing file", "[csv]")
{
    int ret = sample_fsv("/nonexistent/path.csv");
    CHECK(ret == 1);
}

TEST_CASE("sample_fsv returns 1 on malformed CSV", "[csv]")
{
    auto tmp = fs::temp_directory_path() / "test_bad.csv";
    std::ofstream(tmp) << "not,the,right,columns\nx,y,z,w\n";
    REQUIRE(fs::exists(tmp));

    int ret = sample_fsv(tmp.string());
    CHECK(ret == 1);

    fs::remove(tmp);
}

TEST_CASE("sample_fsv handles empty CSV gracefully", "[csv]")
{
    auto tmp = fs::temp_directory_path() / "test_empty.csv";
    std::ofstream(tmp) << "vendor,size,speed\n";
    REQUIRE(fs::exists(tmp));

    int ret = sample_fsv(tmp.string());
    CHECK(ret == 0);

    fs::remove(tmp);
}
