#ifndef STOCK_HPP
#define STOCK_HPP

#include <vector>
#include <string>

struct Hist_Data {
    std::vector<std::string> dates;
    std::vector<double> prices;
};

struct Stock {
    std::string attackN;
    std::string tickerN;
    std::string disclosureD;
    std::vector<Hist_Data> histD;
};

#endif //STOCK_HPP
