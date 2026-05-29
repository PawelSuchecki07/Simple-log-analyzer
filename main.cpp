#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

struct LogEntry {
    std::string raw;
    std::string level;
    std::string timestamp;
    std::string ip;
    std::string message;
    bool validTimestamp = false;
    bool validIp = false;
    bool hasIp = false;
};

bool validateTimestamp(const std::string& ts) {
    std::regex pattern(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    if (!std::regex_match(ts, pattern)) return false;

    int year  = std::stoi(ts.substr(0, 4));
    int month = std::stoi(ts.substr(5, 2));
    int day   = std::stoi(ts.substr(8, 2));
    int hour  = std::stoi(ts.substr(11, 2));
    int min   = std::stoi(ts.substr(14, 2));
    int sec   = std::stoi(ts.substr(17, 2));

    return year >= 1970 && month >= 1 && month <= 12 &&
           day >= 1 && day <= 31 && hour <= 23 &&
           min <= 59 && sec <= 59;
}

bool validateIp(const std::string& ip) {
    std::regex pattern(R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))");
    std::smatch match;
    if (!std::regex_match(ip, match, pattern)) return false;
    for (int i = 1; i <= 4; ++i)
        if (std::stoi(match[i].str()) > 255) return false;
    return true;
}

LogEntry parseLine(const std::string& line) {
    LogEntry entry;
    entry.raw = line;

    // Expected format: YYYY-MM-DD HH:MM:SS [LEVEL] (IP) message
    // Example:         2024-01-15 08:23:11 [ERROR] (192.168.1.1) Disk full
    std::regex fullPattern(
        R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) \[(ERROR|WARNING|INFO)\](?: \((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\))? (.+))"
    );
    std::smatch m;
    if (std::regex_search(line, m, fullPattern)) {
        entry.timestamp     = m[1];
        entry.level         = m[2];
        entry.validTimestamp = validateTimestamp(entry.timestamp);
        if (m[3].matched) {
            entry.hasIp  = true;
            entry.ip     = m[3];
            entry.validIp = validateIp(entry.ip);
        }
        entry.message = m[4];
    }
    return entry;
}

void printSeparator() {
    std::cout << std::string(60, '-') << "\n";
}

int main(int argc, char* argv[]) {
    std::string filename = "sample.log";
    if (argc >= 2) filename = argv[1];

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file '" << filename << "'\n";
        return 1;
    }

    std::vector<LogEntry> errors, warnings, malformed;
    int totalLines = 0;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        ++totalLines;

        LogEntry entry = parseLine(line);

        bool isKnownLevel = (entry.level == "ERROR" || entry.level == "WARNING");
        bool isMalformed  = isKnownLevel && (!entry.validTimestamp || (entry.hasIp && !entry.validIp));

        if (isMalformed) {
            malformed.push_back(entry);
        } else if (entry.level == "ERROR") {
            errors.push_back(entry);
        } else if (entry.level == "WARNING") {
            warnings.push_back(entry);
        }
    }

    // Report
    std::cout << "\n=== LOG ANALYZER REPORT ===\n";
    std::cout << "File: " << filename << "\n";
    std::cout << "Total lines parsed: " << totalLines << "\n";
    printSeparator();

    std::cout << "ERRORS found: " << errors.size() << "\n";
    for (const auto& e : errors) {
        std::cout << "  [" << e.timestamp << "]";
        if (e.hasIp) std::cout << " (" << e.ip << ")";
        std::cout << " " << e.message << "\n";
    }

    printSeparator();
    std::cout << "WARNINGS found: " << warnings.size() << "\n";
    for (const auto& w : warnings) {
        std::cout << "  [" << w.timestamp << "]";
        if (w.hasIp) std::cout << " (" << w.ip << ")";
        std::cout << " " << w.message << "\n";
    }

    printSeparator();
    std::cout << "MALFORMED entries: " << malformed.size() << "\n";
    for (const auto& bad : malformed) {
        std::cout << "  >> " << bad.raw << "\n";
        if (!bad.validTimestamp)
            std::cout << "     ^ invalid timestamp format\n";
        if (bad.hasIp && !bad.validIp)
            std::cout << "     ^ invalid IP address\n";
    }

    printSeparator();
    std::cout << "Summary: " << errors.size() << " error(s), "
              << warnings.size() << " warning(s), "
              << malformed.size() << " malformed line(s)\n\n";

    return 0;
}
