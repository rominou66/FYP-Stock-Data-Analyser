#include "../include/analysis.hpp"

/**
 * @brief Calculate returns from prices
 * 
 * @param prices 
 * @return std::vector<double> 
 */
std::vector<double> calculateReturns(const std::vector<double>& prices) {
    std::vector<double> returns(prices.size(), 0.0);
    for (size_t i = 1; i < prices.size(); ++i) {
        returns[i] = (prices[i] - prices[i - 1]) / prices[i - 1];
    }
    return returns;
}

/**
 * @brief Estimate alpha and beta using returns
 * 
 * @param stockReturns 
 * @param marketReturns 
 */
void estimateAlphaBeta(Stock& stockReturns, Stock& marketReturns) {
    // Extract prices from the first HistoricalData entry
    const auto& stockPrices = stockReturns.histD[0].prices;
    const auto& marketPrices = marketReturns.histD[0].prices;

    // Calculate returns
    std::vector<double> stockReturnsVec = calculateReturns(stockPrices);
    std::vector<double> marketReturnsVec = calculateReturns(marketPrices);

    // Calculate means of returns
    double meanStock = std::accumulate(stockReturnsVec.begin(), stockReturnsVec.end(), 0.0) / stockReturnsVec.size();
    double meanMarket = std::accumulate(marketReturnsVec.begin(), marketReturnsVec.end(), 0.0) / marketReturnsVec.size();

    // Calculate covariance and variance
    double covariance = 0.0;
    double variance = 0.0;
    for (size_t i = 1; i < stockReturnsVec.size(); ++i) {
        covariance += (stockReturnsVec[i] - meanStock) * (marketReturnsVec[i] - meanMarket);
        variance += std::pow(marketReturnsVec[i] - meanMarket, 2);
    }

    // Calculate beta and alpha
    stockReturns.beta = covariance / variance;
    stockReturns.alpha = meanStock - stockReturns.beta * meanMarket;
}
