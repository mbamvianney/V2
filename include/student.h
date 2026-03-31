#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Student {
public:
    std::string firstName;
    std::string lastName;
    std::vector<int> homework;
    int exam{0};
    double finalAverage{0.0};
    double finalMedian{0.0};

    Student() = default;
    Student(const Student&) = default;
    Student(Student&&) noexcept = default;
    Student& operator=(const Student&) = default;
    Student& operator=(Student&&) noexcept = default;
    ~Student() = default;

    [[nodiscard]] bool passedByAverage() const noexcept { return finalAverage >= 5.0; }
    [[nodiscard]] bool passedByMedian() const noexcept { return finalMedian >= 5.0; }

    void computeFinalGrades() {
        finalAverage = 0.4 * averageHomework() + 0.6 * static_cast<double>(exam);
        finalMedian = 0.4 * medianHomework() + 0.6 * static_cast<double>(exam);
    }

    [[nodiscard]] double averageHomework() const {
        if (homework.empty()) {
            return 0.0;
        }
        const double sum = std::accumulate(homework.begin(), homework.end(), 0.0);
        return sum / static_cast<double>(homework.size());
    }

    [[nodiscard]] double medianHomework() const {
        if (homework.empty()) {
            return 0.0;
        }
        std::vector<int> copy = homework;
        std::sort(copy.begin(), copy.end());
        const size_t n = copy.size();
        if (n % 2 == 0) {
            return (static_cast<double>(copy[n / 2 - 1]) + static_cast<double>(copy[n / 2])) / 2.0;
        }
        return static_cast<double>(copy[n / 2]);
    }
};

inline std::istream& operator>>(std::istream& in, Student& student) {
    student = Student{};

    std::string line;
    if (!std::getline(in >> std::ws, line)) {
        return in;
    }
    if (line.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(line);
    if (!(iss >> student.firstName >> student.lastName)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::vector<int> grades;
    int value = 0;
    while (iss >> value) {
        grades.push_back(value);
    }

    if (grades.empty()) {
        throw std::runtime_error("Student line must contain at least an exam grade.");
    }

    student.exam = grades.back();
    grades.pop_back();
    student.homework = std::move(grades);
    student.computeFinalGrades();
    return in;
}

inline std::ostream& operator<<(std::ostream& out, const Student& student) {
    out << std::left << std::setw(16) << student.firstName
        << std::setw(18) << student.lastName
        << std::fixed << std::setprecision(2)
        << std::setw(18) << student.finalAverage
        << std::setw(18) << student.finalMedian;
    return out;
}
