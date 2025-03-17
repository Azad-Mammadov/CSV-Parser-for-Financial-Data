#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::ifstream file("C:\\Users\\AzadMammadov\\Documents\\GitHub\\CSV Parser for Financial Data\\CSV_Files\\input.csv");
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, ','))
        {
            std::cout << field <<" ";
        }
        std::cout << std::endl;
    }

    file.close(); // for closing opened file
    return 0;


}

