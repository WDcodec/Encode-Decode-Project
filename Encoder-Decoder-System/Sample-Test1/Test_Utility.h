#pragma once
#include "setup_utilities.h"

// Helper function to create an empty file
static inline void createEmptyFile(const std::string& file_name) {
    std::ofstream file(file_name, std::ios::out | std::ios::binary);
    // The file is created empty
}

// Setup and teardown functions for logging
static inline void setupTestEnvironment() {
    wd_codec::Logger::init();
}

// Helper function to compare files
bool compare_files(const std::string& file1, const std::string& file2) {
	std::ifstream f1(file1, std::ios::binary | std::ios::ate);
	std::ifstream f2(file2, std::ios::binary | std::ios::ate);

	if (!f1.good() || !f2.good()) return false;

	auto size1 = f1.tellg();
	auto size2 = f2.tellg();

	if (size1 != size2) return false;

	f1.seekg(0, std::ios::beg);
	f2.seekg(0, std::ios::beg);

	std::vector<char> buffer1(size1);
	std::vector<char> buffer2(size2);

	f1.read(buffer1.data(), size1);
	f2.read(buffer2.data(), size2);

	return std::equal(buffer1.begin(), buffer1.end(), buffer2.begin());
}