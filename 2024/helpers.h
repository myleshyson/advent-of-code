#pragma once

#include <fstream>
#include <climits>
#include <string>
#include <vector>
#include <filesystem>
#include <string>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ranges>
#include <print>
#include <ostream>
#include <format>

std::ifstream getStream(const std::string &filename);

FILE *getScan(const char *filename);

template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "[";
    if (vec.size()) {
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i + 1 < vec.size()) std::cout << ", ";
        }
    }
    std::cout << "]";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& vec) {
    // Opening bracket
    os << "[";
    os << std::endl;

    // Loop through elements
    for (size_t i = 0; i < vec.size(); ++i) {
        for (auto j = 0; j < vec[i].size(); j++) {
            os << "[";
            os << vec[i][j];
            // Add comma after all elements except the last
            if (j != vec[i].size() - 1) {
                os << ", ";
            }
            os << "]";
        }
        os << std::endl;
    }

    os << "]";
    // Closing bracket
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    // Opening bracket
    os << "[";

    // Loop through elements
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        // Add comma after all elements except the last
        if (i != vec.size() - 1) {
            os << ", ";
        }
    }

    // Closing bracket
    os << "]";
    return os;
}

template<typename T>
struct std::formatter<std::vector<T>> : std::formatter<string_view> {
    // Parse the format string
    constexpr auto parse(format_parse_context& ctx) {
        // Use the same parsing as string_view
        return formatter<string_view>::parse(ctx);
    }

    // Format the vector
    auto format(const vector<T>& vec, format_context& ctx) const {
        // Start with opening bracket
        format_to(ctx.out(), "[");

        // Add each element
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) {
                format_to(ctx.out(), ", ");
            }
            format_to(ctx.out(), "{}", vec[i]);
        }

        // Close with bracket
        return format_to(ctx.out(), "]");
    }
};