#pragma once

#include "student.h"

#include <algorithm>
#include <deque>
#include <list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

enum class GradeMode { Average = 1, Median = 2 };
enum class Strategy { TwoNewContainers = 1, OneNewContainer = 2 };

template <typename Container>
struct SplitResult {
    Container passed;
    Container failed;
};

inline bool isPassing(const Student& student, GradeMode mode) {
    return mode == GradeMode::Average ? student.passedByAverage() : student.passedByMedian();
}

template <typename Container>
SplitResult<Container> splitStrategy1(const Container& students, GradeMode mode) {
    SplitResult<Container> result;
    std::copy_if(students.begin(), students.end(), std::back_inserter(result.passed),
                 [mode](const Student& s) { return isPassing(s, mode); });
    std::copy_if(students.begin(), students.end(), std::back_inserter(result.failed),
                 [mode](const Student& s) { return !isPassing(s, mode); });
    return result;
}

template <typename Container>
SplitResult<Container> splitStrategy2(Container students, GradeMode mode) {
    SplitResult<Container> result;

    if constexpr (std::is_same_v<Container, std::list<Student>>) {
        const auto firstPassed = std::stable_partition(
            students.begin(), students.end(),
            [mode](const Student& s) { return !isPassing(s, mode); });

        result.failed.splice(result.failed.end(), students, students.begin(), firstPassed);
        result.passed.splice(result.passed.end(), students, students.begin(), students.end());
    } else {
        const auto firstPassed = std::stable_partition(
            students.begin(), students.end(),
            [mode](const Student& s) { return !isPassing(s, mode); });

        std::move(students.begin(), firstPassed, std::back_inserter(result.failed));
        std::move(firstPassed, students.end(), std::back_inserter(result.passed));
    }

    return result;
}
