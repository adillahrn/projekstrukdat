#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// Hapus spasi di kedua ujung string
inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// Pecah baris CSV, mendukung field yang menggunakan tanda kutip
inline std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuote = false;
    for (char c : line) {
        if (c == '"') {
            inQuote = !inQuote;
        } else if (c == ',' && !inQuote) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(trim(field));
    return fields;
}

// Cetak garis pemisah
inline void printSeparator(int width = 60) {
    std::string line(width, '-');
    printf("%s\n", line.c_str());
}

#endif // UTILS_H
