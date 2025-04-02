#include "headers/csv_parser.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>

// The ColumnOrderComparator struct is already defined in the header file, so no need to redefine it here.

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    bool insideQuotes = false;

    for (size_t i = 0; i < s.size(); ++i) {
        char currentChar = s[i];

        if (currentChar == '"') {
            insideQuotes = !insideQuotes;
        } else if (currentChar == delimiter && !insideQuotes) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += currentChar;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::map<std::string, std::string, ColumnOrderComparator>> read_csv(std::ifstream& file) {
    std::vector<std::map<std::string, std::string, ColumnOrderComparator>> data;
    std::string line = "";
    std::string header = "";

    // Get the header
    std::getline(file, header);

    if (header.empty()) {
        std::cerr << "Error: Header is empty" << std::endl;
        return {};
    }

    // Split the header to get the column names
    std::vector<std::string> columns = split(header, ',');

    // Define the custom comparator using the column order
    ColumnOrderComparator comparator(columns);

    // Get the data
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::map<std::string, std::string, ColumnOrderComparator> row(comparator);
        std::vector<std::string> values = split(line, ',');

        for (size_t i = 0; i < columns.size(); i++) {
            if (i < values.size()) {
                row[columns[i]] = values[i];
            } else {
                row[columns[i]] = ""; // Handle missing values
            }
        }

        data.push_back(row);
    }

    return data;
}

// Function to calculate moving averages for "Close" and "Open" columns
std::vector<std::map<std::string, double>> calculate_moving_averages(
    const std::vector<std::map<std::string, std::string, ColumnOrderComparator>>& data) {
    std::vector<std::map<std::string, double>> moving_averages;

    for (size_t i = 0; i < data.size(); ++i) {
        double sum_close = 0.0;
        double sum_open = 0.0;
        int count = 0;

        // Calculate the sum for the 5-day window
        for (size_t j = i; j < data.size() && j < i + 5; ++j) {
            if (data[j].find("Close") != data[j].end() && !data[j].at("Close").empty()) {
                sum_close += std::stod(data[j].at("Close"));
            }
            if (data[j].find("Open") != data[j].end() && !data[j].at("Open").empty()) {
                sum_open += std::stod(data[j].at("Open"));
            }
            ++count;
        }

        // Calculate the moving averages
        std::map<std::string, double> averages;
        averages["Close"] = count > 0 ? sum_close / count : 0.0;
        averages["Open"] = count > 0 ? sum_open / count : 0.0;

        moving_averages.push_back(averages);
    }

    return moving_averages;
}

