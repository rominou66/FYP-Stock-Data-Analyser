#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <iostream>

#include "stock.hpp"

Stock parseCSV(const std::string& path, const std::string& name);
std::vector<Stock> processAllCSVFiles();

#endif //PARSER_HPP
