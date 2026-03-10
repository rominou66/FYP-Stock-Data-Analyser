#ifndef STOCK_HPP
#define STOCK_HPP

#include <vector>
#include <string>

/**
 * @brief struct Hist_Data containing all date/price relationship
 *
 */
struct Hist_Data {
    std::vector<std::string> dates;
    std::vector<double> prices;
};

/**
 * @brief struct Stock containing stock info and Historical data
 *
 */
struct Stock {
    std::string attackN;
    std::string tickerN;
    std::string disclosureD;
    std::vector<Hist_Data> histD;
};

#endif //STOCK_HPP
