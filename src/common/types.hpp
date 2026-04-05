#pragma once
#include <cstdint>

namespace db {
    using page_id_t = uint32_t;
    static constexpr page_id_t INVALID_PAGE_ID = 0xFFFFFFFF;
}