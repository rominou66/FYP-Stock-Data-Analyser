#include "../include/stock.hpp"

/**
 * @brief Get the vector index for a given ticker name
 * 
 * @param stocks 
 * @param name 
 * @return int 
 */
int findStockByName(const std::vector<Stock>& stocks, const std::string& name) {
    for (size_t i = 0; i < stocks.size(); ++i) {
        if (stocks[i].tickerN == name) {
            return i; // Return the index if found
        }
    }
    return -1; // Return -1 if not found
}
