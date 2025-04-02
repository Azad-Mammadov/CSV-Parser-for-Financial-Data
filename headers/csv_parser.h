#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm> // Ensure std::find is recognized

// Custom comparator for std::map to preserve column order
struct ColumnOrderComparator {
    std::vector<std::string> columnOrder;

    ColumnOrderComparator(const std::vector<std::string>& order) : columnOrder(order) {}

    bool operator()(const std::string& a, const std::string& b) const {
        auto itA = std::find(columnOrder.begin(), columnOrder.end(), a);
        auto itB = std::find(columnOrder.begin(), columnOrder.end(), b);
        return itA < itB; // Compare based on the order in columnOrder
    }
};

// Function to split a string by a delimiter
std::vector<std::string> split(const std::string& s, char delimiter);

// Function to read a CSV file and return a list of dictionaries
std::vector<std::map<std::string, std::string, ColumnOrderComparator>> read_csv(std::ifstream& file);

// Function to calculate moving averages for "Close" and "Open" columns
std::vector<std::map<std::string, double>> calculate_moving_averages(
    const std::vector<std::map<std::string, std::string, ColumnOrderComparator>>& data);

#endif // CSV_PARSER_H