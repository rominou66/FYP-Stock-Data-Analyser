#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>


#include "stock.hpp"

Stock parseCSV(const std::string& path, const std::string& name);
std::vector<Stock> processAllCSVFiles();
void savePlottedDataToCSV(const std::string& filename, const std::vector<std::string>& dates, const std::vector<double>& y_values, const std::string& stock_name, int before, int after);
void savePlottedDataToCSV( const std::string& filename, const std::vector<std::string>& stock_names, const std::vector<std::vector<std::string>>& dates_list, const std::vector<std::vector<double>>& y_values_list, int before, int after);

#endif //PARSER_HPP
