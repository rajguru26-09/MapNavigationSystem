#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace Colors {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";
    const std::string BOLD_CYAN  = "\033[1;36m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_YELLOW = "\033[1;33m";
}

namespace Utils {

    // Convert string to lower-case for case-insensitive comparisons
    inline std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return result;
    }

    // Trim leading and trailing whitespace
    inline std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    // Format meters to meters or kilometers
    inline std::string formatDistance(double meters) {
        std::ostringstream ss;
        if (meters >= 1000.0) {
            ss << std::fixed << std::setprecision(2) << (meters / 1000.0) << " km";
        } else {
            ss << std::fixed << std::setprecision(0) << meters << " m";
        }
        return ss.str();
    }

    // Format minutes to minutes and seconds
    inline std::string formatTime(double minutes) {
        std::ostringstream ss;
        if (minutes < 1.0) {
            int secs = static_cast<int>(minutes * 60.0);
            ss << secs << " sec";
        } else if (minutes < 60.0) {
            int mins = static_cast<int>(minutes);
            int secs = static_cast<int>((minutes - mins) * 60.0);
            if (secs > 0) {
                ss << mins << " min " << secs << " sec";
            } else {
                ss << mins << " min";
            }
        } else {
            int hrs = static_cast<int>(minutes / 60.0);
            int mins = static_cast<int>(minutes) % 60;
            ss << hrs << " hr " << mins << " min";
        }
        return ss.str();
    }

    // Print divider
    inline void printDivider(char ch = '=', int len = 65) {
        std::cout << Colors::CYAN << std::string(len, ch) << Colors::RESET << "\n";
    }

} // namespace Utils

#endif // UTILS_H
