#pragma once

#include <string>
#include <fstream>
#include <span>
#include <spdlog/spdlog.h>

#include "common/config.hpp"
#include "common/types.hpp"

namespace db::storage {

class DiskManager {
public:
    explicit DiskManager(const std::string& db_path);

    void write_page(page_id_t page_id, std::span<const char, PAGE_SIZE> buffer);
    void read_page(page_id_t page_id, std::span<char, PAGE_SIZE> buffer);
    page_id_t allocate_page();
    page_id_t get_number_of_pages();

private:
    std::fstream db_io_;
};

} // namespace db::storage