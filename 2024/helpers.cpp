#include "helpers.h"


std::string projectPath = std::filesystem::path(__FILE__).parent_path();

FILE *getScan(const char *filename) {
    const std::string path(projectPath + "/" + filename);
    FILE *f = fopen(path.c_str(), "r");

    return f;
}

std::ifstream getStream(const std::string &filename) {
    std::ifstream infile(projectPath + "/" + filename);

    return infile;
}

std::vector<std::string> explode(const std::string &string, const std::string &delimiter) {
    std::vector<std::string> results;
    size_t start = 0;

    if (string.find(delimiter) == std::string::npos) return results;

    while (start < string.length()) {
        size_t end = string.find(delimiter, start);

        if (end == std::string::npos) {
            end = string.length();
        }

        results.push_back(string.substr(start, end - start));
        start = end + delimiter.length();
    }

    return results;
}
