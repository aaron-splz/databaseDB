#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <span>

const int PAGE_SIZE = 4096; //ToDo: Store in seperate constants file

namespace db::storage{

class DiskManager{

public:
    explicit DiskManager(const std::string& db_path){

        //Try open the file with read,write permissions and no interpretation of the data
        db_io_.open(db_path, std::ios::in | std::ios::out | std::ios::binary);

        //if open failed create a new file and open
        if(!db_io_.is_open()){
            db_io_.open(db_path, std::ios::trunc | std::ios::out | std::ios::binary);
            db_io_.close();
            
            db_io_.open(db_path, std::ios::in | std::ios::out | std::ios::binary);
        }
        
    }

    // Write a page to the database
    void write_page(const u_int32_t page_id, const char* payload){ //ToDo: maybe define Datatype for page indices
        size_t page_offset = page_id * PAGE_SIZE;
        db_io_.seekp(page_offset);
        db_io_.write(payload, PAGE_SIZE);

        db_io_.flush();
    }

    // read a page from the database. We use span to make sure we have the right size
    void read_page(size_t page_id, std::span<char, PAGE_SIZE> buffer) {
        db_io_.seekg(page_id * PAGE_SIZE);
        db_io_.read(buffer.data(), PAGE_SIZE);
    }   



private:
    std::fstream db_io_;
};

} //namespace db::storage