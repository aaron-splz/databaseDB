#include "storage/disk_manager.hpp"

namespace db::storage {

DiskManager::DiskManager(const std::string& db_path) {
    spdlog::info("Opening database file: {}", db_path);

    //Try open the file with read,write permissions and no interpretation of the data
    db_io_.open(db_path, std::ios::in | std::ios::out | std::ios::binary);

    //if open failed create a new file and open
    if(!db_io_.is_open()){
        spdlog::warn("File not found, creating: {}", db_path);
        db_io_.open(db_path, std::ios::trunc | std::ios::out | std::ios::binary);
        db_io_.close();
        
        db_io_.open(db_path, std::ios::in | std::ios::out | std::ios::binary);
        
        if(!db_io_.is_open()){
            spdlog::error("Critical: Could not create file {}", db_path);
        }
    }
}

// Write a page to the database
void DiskManager::write_page(page_id_t page_id, std::span<const char, PAGE_SIZE> buffer) { //ToDo: maybe define Datatype for page indices
    size_t page_offset = static_cast<size_t>(page_id) * PAGE_SIZE;
    spdlog::debug("Writing page {} at offset {}", page_id, page_offset);
    
    db_io_.seekp(page_offset);
    db_io_.write(buffer.data(), PAGE_SIZE);

    db_io_.flush();
}

// read a page from the database. We use span to make sure we have the right size
void DiskManager::read_page(page_id_t page_id, std::span<char, PAGE_SIZE> buffer) {
    size_t page_offset = static_cast<size_t>(page_id) * PAGE_SIZE;
    spdlog::debug("Reading page {} from offset {}", page_id, page_offset);

    db_io_.seekg(page_offset);
    db_io_.read(buffer.data(), PAGE_SIZE);
}

//allocate a new page
page_id_t DiskManager::allocate_page() {
    db_io_.seekg(0, std::ios::end);
    size_t file_size = static_cast<size_t>(db_io_.tellg());
    page_id_t new_page_id = static_cast<page_id_t>(file_size / PAGE_SIZE);

    char empty_byte = 0;
    db_io_.seekp((new_page_id + 1) * PAGE_SIZE - 1);
    db_io_.write(&empty_byte, 1);
    db_io_.flush();

    spdlog::info("Allocated new page: {}", new_page_id);
    return new_page_id;
}

page_id_t DiskManager::get_number_of_pages() {
    db_io_.seekg(0, std::ios::end);
    size_t file_size = static_cast<size_t>(db_io_.tellg());
    return static_cast<page_id_t>(file_size / PAGE_SIZE);
}

} // namespace db::storage