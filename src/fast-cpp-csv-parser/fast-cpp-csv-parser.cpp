#include "fast-cpp-csv-parser.h"

#include <vector>
#include <string>

#include <spdlog/spdlog.h>

#include "csv.h"

struct RAMModule {
    std::string vendor;
    int size;
    double speed;
};

int sample_fsv(const std::string& csv_path)
{
    spdlog::info("{} opening CSV: {}", kFastTag, csv_path);

    std::vector<RAMModule> modules;
    try {
        io::CSVReader<3> in(csv_path);
        in.read_header(io::ignore_extra_column, "vendor", "size", "speed");
        spdlog::info("{} header parsed (vendor, size, speed)", kFastTag);

        std::string vendor;
        int size;
        double speed;

        while (in.read_row(vendor, size, speed)) {
            modules.push_back({vendor, size, speed});
            spdlog::debug("{} row: vendor={} size={}GB speed={}MHz",
                          kFastTag, vendor, size, speed);
        }

        spdlog::info("{} read {} RAM record(s)", kFastTag, modules.size());
        for (const auto& m : modules) {
            spdlog::info("{} record: vendor={} size={}GB speed={}MHz",
                         kFastTag, m.vendor, m.size, m.speed);
        }

    } catch (const std::exception& e) {
        spdlog::error("{} error reading CSV file '{}': {}",
                      kFastTag, csv_path, e.what());
        return 1;
    }
    return 0;
}
