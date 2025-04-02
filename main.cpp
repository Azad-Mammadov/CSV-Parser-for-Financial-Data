#include "headers/csv_parser.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h> // For GetModuleFileName

// Function to get the directory of the executable
std::string get_executable_directory() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    return fullPath.substr(0, fullPath.find_last_of("\\/")); // Extract directory
}

void process_file(const std::string& exeDirectory) {
    while (true) {
        // Ask the user for the name of the CSV file (without .csv extension)
        std::string fileNameWithoutExtension;
        std::cout << "Enter the name of the CSV file (without .csv): ";
        std::cin >> fileNameWithoutExtension;

        // Construct the full path to the input file
        std::string filename = exeDirectory + "\\" + fileNameWithoutExtension + ".csv";

        // Open the CSV file
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file: " << filename << std::endl;
            std::cout << "Do you want to try again? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'n' || choice == 'N') {
                return; // Exit the function
            }
            continue; // Retry
        }

        // Read the CSV file using the parser
        auto data = read_csv(file);

        if (data.empty()) {
            std::cerr << "Error: No data found in the file." << std::endl;
            std::cout << "Do you want to try again? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'n' || choice == 'N') {
                return; // Exit the function
            }
            continue; // Retry
        }

        // Print the data
        std::cout << "Parsed Data:\n";
        for (const auto& row : data) {
            for (const auto& [key, value] : row) {
                std::cout << key << ": " << value << " ";
            }
            std::cout << std::endl;
        }

        // Calculate moving averages
        auto moving_averages = calculate_moving_averages(data);

        // Print the moving averages
        std::cout << "\n5-Day Moving Averages:\n";
        for (size_t i = 0; i < moving_averages.size(); ++i) {
            std::cout << "Day " << i + 1 << " - Close: " << moving_averages[i]["Close"]
                      << ", Open: " << moving_averages[i]["Open"] << std::endl;
        }

        // Ask if the user wants to read another file
        std::cout << "\nDo you want to read another file? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'n' || choice == 'N') {
            return; // Exit the function
        }
    }
}

int main() {
    // Get the directory of the executable
    std::string exeDirectory = get_executable_directory();

    // Process files in a loop
    process_file(exeDirectory);

    // Wait for the user to press Enter before closing
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(); // Clear the newline left in the input buffer
    std::cin.get();

    return 0;
}

