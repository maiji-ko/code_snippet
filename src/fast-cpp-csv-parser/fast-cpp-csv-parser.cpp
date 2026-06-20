#include "fast-cpp-csv-parser.h"

#include <vector>
#include <string>
#include <iostream>

#include "csv.h"

struct RAMModule {
    std::string vendor;
    int size;
    double speed;
};

int sample_fsv(const std::string& csv_path)
{
    std::vector<RAMModule> modules;
    try {
        io::CSVReader<3> in(csv_path);
        in.read_header(io::ignore_extra_column, "vendor", "size", "speed");

        std::string vendor;
        int size;
        double speed;

        while(in.read_row(vendor, size, speed)) {
            modules.push_back({vendor, size, speed});
        }

        // 显示读取结果
        std::cout << "Successfully read " << modules.size() << " RAM record\n\n";

        for(const auto& m : modules) {
            std::cout << "Vendor: " << m.vendor
                      << ", Size: " << m.size << "GB"
                      << ", Speed: " << m.speed << "MHz\n";
        }

    } catch(const std::exception& e) {
        std::cerr << "Error reading CSV file: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}