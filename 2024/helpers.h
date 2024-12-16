#pragma once
#include <fstream>

std::ifstream getStream(const std::string &filename);

FILE *getScan(const char *filename);
