// Helper function to create an empty file
void createEmptyFile(const std::string& file_name) {
    std::ofstream file(file_name, std::ios::out | std::ios::binary);
    // The file is created empty
}
//
//// Setup and teardown functions for logging
void setupTestEnvironment() {
    wd_codec::Logger::init();
}

void teardownTestEnvironment() {
    wd_codec::Logger::close();
}