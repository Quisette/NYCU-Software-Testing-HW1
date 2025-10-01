#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <numeric>

// ANSI color codes for better output
const char* const GREEN = "\033[92m";
const char* const RED = "\033[91m";
const char* const YELLOW = "\033[93m";
const char* const RESET = "\033[0m";

bool compile_cpp(const std::string& source, const std::string& output) {
    std::cout << "  Compiling " << source << "..." << std::endl;
    // Use clang++ on macOS and add -g for debug symbols needed by `leaks`
    std::string command = "g++-15 -std=c++11 -g -pthread " + source + " -o " + output + " -isysroot $(xcrun --show-sdk-path) -fno-permissive -fpermissive ";
    if (std::system(command.c_str()) != 0) {
        std::cerr << RED << "  Compilation failed." << RESET << std::endl;
        return false;
    }
    return true;
}

// Reads an entire file into a string
std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Return codes: 1=pass, 0=fail, -1=skip
int test_deadlock() {
    std::cout << "1. Testing for Deadlock (ResourceScheduler.py)" << std::endl;
    if (std::system("command -v gtimeout > /dev/null 2>&1") != 0) {
        std::cout << YELLOW << "  SKIP:" << RESET << " `gtimeout` not found. Run `brew install coreutils`." << std::endl;
        return -1;
    }
    // `timeout` from coreutils is installed as `gtimeout` on macOS to avoid conflict.
    int status = std::system("gtimeout 5s python3 tests/test_deadlock.py > evidence/deadlock_output.txt 2>&1");
    if (WEXITSTATUS(status) == 124) {
        std::cout << GREEN << "  PASS:" << RESET << " Program timed out as expected, indicating a deadlock." << std::endl;
        std::cout << "  Evidence saved in: evidence/deadlock_output.txt" << std::endl;
        return 1;
    } else {
        std::cout << RED << "  FAIL:" << RESET << " Program finished unexpectedly (exit code " << WEXITSTATUS(status) << ")." << std::endl;
        return 0;
    }
}

int test_memory_leak() {
    std::cout << "2. Testing for Memory Leak (DataProcessor.cpp)" << std::endl;
    if (std::system("MallocStackLogging=1 command -v leaks > /dev/null 2>&1") != 0) {
        std::cout << YELLOW << "  SKIP:" << RESET << " `leaks` tool not found. (Install Xcode Command Line Tools)." << std::endl;
        return -1;
    }

    if (!compile_cpp("./prob_src/DataProcessor.cpp", "data_processer")) return 0;

    std::system("leaks -atExit -- ./data_processer | head -n 50 > evidence/leaks_output.txt 2>&1");
    std::string output = read_file_contents("evidence/leaks_output.txt");

    // Check if the output contains a leak report that isn't the "0 leaks" message.
    if ((output.find("leaked bytes") != std::string::npos || output.find("leaks for") != std::string::npos) &&
        output.find("0 leaks for 0") == std::string::npos) {
        std::cout << GREEN << "  PASS:" << RESET << " `leaks` tool detected memory leaks as expected." << std::endl;
        std::cout << "  Evidence saved in: evidence/leaks_output.txt" << std::endl;
        return 1;
    } else {
        std::cout << RED << "  FAIL:" << RESET << " `leaks` tool did not report any memory leaks." << std::endl;
        return 0;
    }
}

int test_race_condition() {
    std::cout << "3. Testing for Race Condition (LoggingSystem.cpp)" << std::endl;
    if (!compile_cpp("./prob_src/LoggingSystem.cpp", "logging_system")) return 0;

    std::system("./logging_system > evidence/race_output.txt");

    std::ifstream file("evidence/race_output.txt");
    std::string line;
    long expected_total = -1, final_total = -1;

    while (std::getline(file, line)) {
        if (line.find("Expected total logs:") != std::string::npos) {
            expected_total = std::stol(line.substr(line.find(":") + 1));
        }
        if (line.find("Final count of logs processed:") != std::string::npos) {
            final_total = std::stol(line.substr(line.find(":") + 1));
        }
    }

    if (expected_total != -1 && final_total != -1 && final_total < expected_total) {
        std::cout << GREEN << "  PASS:" << RESET << " Final count (" << final_total << ") is less than expected (" << expected_total << ")." << std::endl;
        std::cout << "  Evidence saved in: evidence/race_output.txt" << std::endl;
        return 1;
    } else {
        std::cout << RED << "  FAIL:" << RESET << " Final count (" << final_total << ") was not less than expected (" << expected_total << ")." << std::endl;
        return 0;
    }
}

int test_buffer_overflow() {
    std::cout << "4. Testing for Buffer Overflow (ProfileUpdater.cpp)" << std::endl;
    if (!compile_cpp("./tests/test_buffer_overflow.cpp", "test_buffer_overflow")) return 0;

    int status = std::system("./test_buffer_overflow > evidence/overflow_output.txt 2>&1");
    std::string output = read_file_contents("evidence/overflow_output.txt");

    // Check if program crashed (non-zero exit) OR if data was corrupted
    bool crashed = (status != 0);
    auto after_pos = output.find("--- Profile After Malicious Update ---");
    bool id_corrupt_after = after_pos != std::string::npos && output.substr(after_pos).find("User ID: 12345") == std::string::npos;

    if (crashed || id_corrupt_after) {
        if (crashed) {
            std::cout << GREEN << "  PASS:" << RESET << " Program crashed due to buffer overflow as expected." << std::endl;
        } else {
            std::cout << GREEN << "  PASS:" << RESET << " Profile data was corrupted as expected after the update." << std::endl;
        }
        std::cout << "  Evidence saved in: evidence/overflow_output.txt" << std::endl;
        return 1;
    } else {
        std::cout << RED << "  FAIL:" << RESET << " Profile data does not appear to be corrupted and program did not crash." << std::endl;
        return 0;
    }
}

int test_const_violation() {
    std::cout << "5. Testing for Const Violation Crash (MatrixProcessor.cpp)" << std::endl;
    if (!compile_cpp("./tests/test_const_violation.cpp", "test_const_violation")) return 0;

    int status = std::system("./test_const_violation > evidence/const_violation_output.txt 2>&1");
    std::string output = read_file_contents("evidence/const_violation_output.txt");

    if (status != 0) { // Any non-zero status indicates an abnormal exit (crash)
        std::cout << GREEN << "  PASS:" << RESET << " Program crashed with exit status " << WEXITSTATUS(status) << " as expected." << std::endl;
        std::cout << "  Evidence saved in: evidence/const_violation_output.txt" << std::endl;
        return 1;
    } else {
        std::cout << RED << "  FAIL:" << RESET << " Program completed with exit code 0 (no crash detected)." << std::endl;
        return 0;
    }
}

void cleanup() {
    std::system("rm -f data_processer logging_system test_buffer_overflow test_const_violation");
    // Keep evidence files in evidence/ directory
}

int main() {
    // Create evidence directory for test outputs
    std::system("mkdir -p evidence");

    using TestFunc = int (*)();
    std::vector<TestFunc> tests = {
        test_deadlock,
        test_memory_leak,
        test_race_condition,
        test_buffer_overflow,
        test_const_violation
    };

    int passed_count = 0, failed_count = 0, skipped_count = 0;

    for (auto& test : tests) {
        int result = test();
        if (result == 1) passed_count++;
        else if (result == 0) failed_count++;
        else skipped_count++; // result == -1
        std::cout << std::string(40, '-') << std::endl;
    }

    std::cout << "\n===== Verification Summary =====" << std::endl;
    std::cout << GREEN << "Passed:  " << passed_count << RESET << std::endl;
    std::cout << RED << "Failed:  " << failed_count << RESET << std::endl;
    if (skipped_count > 0) {
        std::cout << YELLOW << "Skipped: " << skipped_count << RESET << std::endl;
    }
    std::cout << "==============================" << std::endl;

    cleanup();
    return (failed_count > 0) ? 1 : 0;
}