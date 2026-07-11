#pragma once

#include <string>
#include <string_view>

inline constexpr std::string_view kFastTag = "[fast_csv]";

int sample_fsv(const std::string& csv_path);