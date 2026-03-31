#include "io_utils.h"
#include "strategies.h"
#include "timer.h"

#include <deque>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct RunSummary {
    std::string containerName;
    std::string strategyName;
    std::string gradeName;
    size_t totalStudents{};
    size_t passed{};
    size_t failed{};
    double readSeconds{};
    double splitSeconds{};
    double writeSeconds{};
    double totalSeconds{};
};

template <typename Container>
RunSummary runCase(const std::string& inputFile,
                   const std::string& containerName,
                   Strategy strategy,
                   GradeMode gradeMode) {
    Timer totalTimer;

    Timer readTimer;
    Container students = io::readStudents<Container>(inputFile);
    const double readSeconds = readTimer.elapsedSeconds();

    Timer splitTimer;
    SplitResult<Container> split = strategy == Strategy::TwoNewContainers
                                       ? splitStrategy1(students, gradeMode)
                                       : splitStrategy2(std::move(students), gradeMode);
    const double splitSeconds = splitTimer.elapsedSeconds();

    const std::string strategyName = strategy == Strategy::TwoNewContainers ? "strategy1" : "strategy2";
    const std::string gradeName = gradeMode == GradeMode::Average ? "average" : "median";

    fs::create_directories("output");

    Timer writeTimer;
    io::writeStudents("output/failed_" + containerName + "_" + strategyName + "_" + gradeName + ".txt",
                      "Failed students", io::toVector(split.failed));
    io::writeStudents("output/passed_" + containerName + "_" + strategyName + "_" + gradeName + ".txt",
                      "Passed students", io::toVector(split.passed));
    const double writeSeconds = writeTimer.elapsedSeconds();

    return {
        containerName,
        strategyName,
        gradeName,
        split.passed.size() + split.failed.size(),
        split.passed.size(),
        split.failed.size(),
        readSeconds,
        splitSeconds,
        writeSeconds,
        totalTimer.elapsedSeconds(),
    };
}

void printSummary(const RunSummary& summary) {
    std::cout << "\n[" << summary.containerName << "] "
              << summary.strategyName << ", grade mode: " << summary.gradeName << '\n'
              << "  Students total : " << summary.totalStudents << '\n'
              << "  Passed         : " << summary.passed << '\n'
              << "  Failed         : " << summary.failed << '\n'
              << "  Read time      : " << summary.readSeconds << " s\n"
              << "  Split time     : " << summary.splitSeconds << " s\n"
              << "  Write time     : " << summary.writeSeconds << " s\n"
              << "  Total time     : " << summary.totalSeconds << " s\n";
}

void showMenu() {
    std::cout << "===== Student Split Benchmark v1.0 =====\n"
              << "1. Vector\n"
              << "2. List\n"
              << "3. Deque\n"
              << "4. All containers\n"
              << "Choose container: ";
}

GradeMode askGradeMode() {
    int choice = 0;
    std::cout << "\nChoose final grade method:\n"
              << "1. Average\n"
              << "2. Median\n"
              << "Your choice: ";
    std::cin >> choice;
    return choice == 2 ? GradeMode::Median : GradeMode::Average;
}

Strategy askStrategy() {
    int choice = 0;
    std::cout << "\nChoose splitting strategy:\n"
              << "1. Two new containers (copy into passed + failed)\n"
              << "2. One new container (partition and keep the other group)\n"
              << "Your choice: ";
    std::cin >> choice;
    return choice == 2 ? Strategy::OneNewContainer : Strategy::TwoNewContainers;
}

int main(int argc, char* argv[]) {
    try {
        std::string inputFile = "studentai.txt";
        if (argc > 1) {
            inputFile = argv[1];
        }

        int containerChoice = 4;
        if (argc > 2) {
            containerChoice = std::stoi(argv[2]);
        } else {
            showMenu();
            std::cin >> containerChoice;
        }

        GradeMode gradeMode = askGradeMode();
        Strategy strategy = askStrategy();

        switch (containerChoice) {
            case 1:
                printSummary(runCase<std::vector<Student>>(inputFile, "vector", strategy, gradeMode));
                break;
            case 2:
                printSummary(runCase<std::list<Student>>(inputFile, "list", strategy, gradeMode));
                break;
            case 3:
                printSummary(runCase<std::deque<Student>>(inputFile, "deque", strategy, gradeMode));
                break;
            case 4:
            default:
                printSummary(runCase<std::vector<Student>>(inputFile, "vector", strategy, gradeMode));
                printSummary(runCase<std::list<Student>>(inputFile, "list", strategy, gradeMode));
                printSummary(runCase<std::deque<Student>>(inputFile, "deque", strategy, gradeMode));
                break;
        }

        std::cout << "\nResults written to ./output\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
