#pragma once

#include <chrono>
#include <string>

class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    [[nodiscard]] double elapsedSeconds() const {
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};
