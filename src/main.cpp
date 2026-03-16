#include <vector>

#include "../include/plotter.hpp"
#include "../include/parser.hpp"
#include "../include/stock.hpp"
#include "../include/analysis.hpp"

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
    int spy = stocks[findStockByName(stocks, "SPY")];
    for(auto& stock : stocks) {
        estimateAlphaBeta(stock, stocks[spy]);
    }
    // Abnormal Return
    for(const auto& stock : stocks) {
        abnormalReturnPlot(stock, stocks[spy]);
    }

    return 0;
}
