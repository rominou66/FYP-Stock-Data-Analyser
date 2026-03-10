#include <vector>

#include "../include/plotter.hpp"
#include "../include/parser.hpp"
#include "../include/stock.hpp"

int main() {
    // 1. Parse all CSV files and get the stock data
    std::vector<Stock> stocks = processAllCSVFiles();

    // 2. Plot each stock's data
    for(const auto& stock : stocks) {
        singlePlot(stock);
    }

    // 3. Plot all stocks to single graph
    multiPlot(stocks);

    // 4. Plot all stocks to single graph normalized
    normalizedMultiPlot(stocks);

    // 5. Analysed

    return 0;
}
