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
