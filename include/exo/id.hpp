#pragma once

#include <random>
#include <string>
#include <uuid.h>

#define DEFAULT_ID_LENGTH 12

namespace exo {
    using Id = uuids::uuid;
    using IdGenerator = uuids::uuid_random_generator;

    static exo::IdGenerator make_id_generator() {
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size> {};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937 generator(seq);
        uuids::uuid_random_generator gen{generator};
        return gen;
    }
}
