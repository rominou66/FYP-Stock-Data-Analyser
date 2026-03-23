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
        singlePlot(stock, 30, 30);
        singlePlot(stock, 0, 5);
    }

    // 3. Plot all stocks to single graph
    multiPlot(stocks, 30, 30);
    multiPlot(stocks, 0, 5);

    // 4. Plot all stocks to single graph normalized
    normalizedMultiPlot(stocks, 30, 30);
    normalizedMultiPlot(stocks, 0, 5);

    // 5. Analysed
    int spy = findStockByName(stocks, "SPY");
    for(auto& stock : stocks) {
        estimateAlphaBeta(stock, stocks[spy]);
    }
    // 5.1 Abnormal Return
    for(const auto& stock : stocks) {
        abnormalReturnPlot(stock, stocks[spy], 30, 30);
        abnormalReturnPlot(stock, stocks[spy], 0, 5);
    }
    // 5.2 Cumulative Abnormal Return
    for(const auto& stock : stocks) {
        cumulativeARPlot(stock, stocks[spy], 30, 30);
        cumulativeARPlot(stock, stocks[spy], 0, 5);
    }

    return 0;
}
