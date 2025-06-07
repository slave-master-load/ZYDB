#pragma once

#include <random>

namespace zydb::base {

class Random {
public:
    Random() : engine(static_cast<unsigned long>(std::time(nullptr))) {}

    int64_t GetInt(int min, int max) const {
        std::uniform_int_distribution<int64_t> dist(min, max);
        return dist(engine);
    }

    double GetDouble() const {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    std::string GetString(int length) const {
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);

        std::string result;
        result.reserve(length);
        for (int i = 0; i < length; ++i) {
            result += charset[dist(engine)];
        }
        return result;
    }

private:
    mutable std::default_random_engine engine;
};

} // namespace zydb::base
