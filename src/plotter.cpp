#include "../include/plotter.hpp"
#include <iostream>
#include <algorithm>

/**
 * @brief Plot individual graph of a single stock, with disclosure date annotation and output an SVG
 *
 * @param stock
 */
void singlePlot(Stock stock) {
    if(stock.histD.empty() || stock.histD[0].dates.empty() || stock.histD[0].prices.empty()) {
        std::cerr << "No data available to plot for stock: " << stock.tickerN << std::endl;
        return;
    }

    // Extract data from the first historical data set
    const auto& dates = stock.histD[0].dates;
    const auto& prices = stock.histD[0].prices;

    // Convert dates to numerical values for plotting
    std::vector<double> x_values(dates.size());
    for(size_t i = 0; i < dates.size(); i++) {
        x_values[i] = static_cast<double>(i);
    }

    // Create the plot
    auto fig = matplot::figure(true);
    matplot::plot(x_values, prices, "-o");

    // Add disclosure date marker if available
    if(!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        // Try to find the disclosure date in the dates vector
        // Convert ddmmyyyy to a comparable format (remove / if present)
        std::string target_date = stock.disclosureD;
        if(target_date.length() == 8) {
            // Format: ddmmyyyy -> dd/mm/yyyy for comparison
            target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
        }

        // Find the index of the disclosure date
        size_t disclosure_index = dates.size(); // default to end if not found
        for(size_t i = 0; i < dates.size(); i++) {
            if(dates[i] == target_date) {
                disclosure_index = i;
                break;
            }
        }

        // Add red text arrow at disclosure date
        if(disclosure_index < dates.size()) {
            double x_disclosure = static_cast<double>(disclosure_index);
            double y_disclosure = prices[disclosure_index];
            auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 2.5, x_disclosure, y_disclosure, "Disclosure Date");
            t->color("red").font_size(14);
            a->color("red");
        }
    }

    std::string title = "Historical Stock Price: " + stock.tickerN;
    if(!stock.attackN.empty() && stock.attackN != "Unknown") {
        title += " (" + stock.attackN + ")";
    }
    if(!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        title += " - Disclosed: " + stock.disclosureD;
    }
    matplot::title(title);
    matplot::xlabel("Time Period");
    matplot::ylabel("Price ($)");
    matplot::grid(true);

    // Add some date labels to the x-axis
    if(!dates.empty()) {
        matplot::xticks({0, static_cast<double>(dates.size()-1)});
        matplot::xticklabels({dates[0], dates.back()});
    }

    matplot::save("../output/" + stock.attackN + "-" + stock.tickerN, "svg");
}

/**
 * @brief Plot all the of the vector of Struct Stock to a single graph
 *
 * @param stocks
 */
void multiPlot(const std::vector<Stock>& stocks) {
    if (stocks.empty()) {
        std::cerr << "No stocks provided to plot." << std::endl;
        return;
    }

    // Check if any stock has valid data
    bool has_valid_data = false;
    for (const auto& stock : stocks) {
        if (!stock.histD.empty() && !stock.histD[0].dates.empty() && !stock.histD[0].prices.empty()) {
            has_valid_data = true;
            break;
        }
    }
    if (!has_valid_data) {
        std::cerr << "No valid data available to plot for any stock." << std::endl;
        return;
    }

    // Create the plot
    auto fig = matplot::figure(true);
    matplot::hold(matplot::on);

    // Plot each stock's data
    for (const auto& stock : stocks) {
        if (stock.histD.empty() || stock.histD[0].dates.empty() || stock.histD[0].prices.empty()) {
            std::cerr << "No data available to plot for stock: " << stock.tickerN << std::endl;
            continue;
        }

        const auto& dates = stock.histD[0].dates;
        const auto& prices = stock.histD[0].prices;

        // Convert dates to numerical values for plotting
        std::vector<double> x_values(dates.size());
        for (size_t i = 0; i < dates.size(); i++) {
            x_values[i] = static_cast<double>(i);
        }

        // Plot the stock data
        matplot::plot(x_values, prices, "-o")->display_name(stock.tickerN);

        // Add disclosure date marker if available
        if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
            std::string target_date = stock.disclosureD;
            if (target_date.length() == 8) {
                target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
            }

            size_t disclosure_index = dates.size();
            for (size_t i = 0; i < dates.size(); i++) {
                if (dates[i] == target_date) {
                    disclosure_index = i;
                    break;
                }
            }

            if (disclosure_index < dates.size()) {
                double x_disclosure = static_cast<double>(disclosure_index);
                double y_disclosure = prices[disclosure_index];
                auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 2.5, x_disclosure, y_disclosure, "Disclosure: " + stock.tickerN);
                t->color("red").font_size(10);
                a->color("red");
            }
        }
    }

    // Set plot title and labels
    std::string title = "Historical Stock Prices";
    matplot::title(title);
    matplot::xlabel("Time Period");
    matplot::ylabel("Price ($)");
    matplot::grid(true);
    matplot::legend();

    // Set x-axis ticks and labels based on the first stock's dates
    if (!stocks[0].histD.empty() && !stocks[0].histD[0].dates.empty()) {
        const auto& dates = stocks[0].histD[0].dates;
        matplot::xticks({0, static_cast<double>(dates.size() - 1)});
        matplot::xticklabels({dates[0], dates.back()});
    }

    // Save the plot
    matplot::save("../output/multi-stock-plot", "svg");
    matplot::hold(matplot::off);
}

/**
 * @brief Plot all the of the vector of Struct Stock to a single graph (Normalized)
 *
 * @param stocks
 */
void normalizedMultiPlot(const std::vector<Stock>& stocks) {
    if (stocks.empty()) {
        std::cerr << "No stocks provided to plot." << std::endl;
        return;
    }

    // Check if any stock has valid data
    bool has_valid_data = false;
    for (const auto& stock : stocks) {
        if (!stock.histD.empty() && !stock.histD[0].dates.empty() && !stock.histD[0].prices.empty()) {
            has_valid_data = true;
            break;
        }
    }
    if (!has_valid_data) {
        std::cerr << "No valid data available to plot for any stock." << std::endl;
        return;
    }

    // Create the plot
    auto fig = matplot::figure(true);
    matplot::hold(matplot::on);

    // Plot each stock's data, normalized
    for (const auto& stock : stocks) {
        if (stock.histD.empty() || stock.histD[0].dates.empty() || stock.histD[0].prices.empty()) {
            std::cerr << "No data available to plot for stock: " << stock.tickerN << std::endl;
            continue;
        }

        const auto& dates = stock.histD[0].dates;
        const auto& prices = stock.histD[0].prices;

        // Convert dates to numerical values for plotting
        std::vector<double> x_values(dates.size());
        for (size_t i = 0; i < dates.size(); i++) {
            x_values[i] = static_cast<double>(i);
        }

        // Normalize prices to [0, 1] range
        double min_price = *std::min_element(prices.begin(), prices.end());
        double max_price = *std::max_element(prices.begin(), prices.end());
        std::vector<double> normalized_prices(prices.size());
        for (size_t i = 0; i < prices.size(); i++) {
            normalized_prices[i] = (prices[i] - min_price) / (max_price - min_price);
        }

        // Plot the normalized stock data
        matplot::plot(x_values, normalized_prices, "-o")->display_name(stock.tickerN);

        // Add disclosure date marker if available
        if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
            std::string target_date = stock.disclosureD;
            if (target_date.length() == 8) {
                target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
            }

            size_t disclosure_index = dates.size();
            for (size_t i = 0; i < dates.size(); i++) {
                if (dates[i] == target_date) {
                    disclosure_index = i;
                    break;
                }
            }

            if (disclosure_index < dates.size()) {
                double x_disclosure = static_cast<double>(disclosure_index);
                double y_disclosure = normalized_prices[disclosure_index];
                auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 0.1, x_disclosure, y_disclosure, "Disclosure: " + stock.tickerN);
                t->color("red").font_size(10);
                a->color("red");
            }
        }
    }

    // Set plot title and labels
    std::string title = "Normalized Historical Stock Price Movements";
    matplot::title(title);
    matplot::xlabel("Time Period");
    matplot::ylabel("Normalized Price");
    matplot::grid(true);
    matplot::legend();

    // Set x-axis ticks and labels based on the first stock's dates
    if (!stocks[0].histD.empty() && !stocks[0].histD[0].dates.empty()) {
        const auto& dates = stocks[0].histD[0].dates;
        matplot::xticks({0, static_cast<double>(dates.size() - 1)});
        matplot::xticklabels({dates[0], dates.back()});
    }

    // Save the plot
    matplot::save("../output/normalized-multi-stock-plot", "svg");
    matplot::hold(matplot::off);
}

/**
 * @brief Single plot for Abnormal Return
 * 
 * @param stock 
 * @param marketReturns 
 */
void abnormalReturnPlot(Stock stock, Stock marketReturns) {
    if (stock.histD.empty() || stock.histD[0].dates.empty() || stock.histD[0].prices.empty()) {
        std::cerr << "No data available to plot for stock: " << stock.tickerN << std::endl;
        return;
    }

    const auto& dates = stock.histD[0].dates;
    const auto& prices = stock.histD[0].prices;
    const auto& marketPrices = marketReturns.histD[0].prices;

    // Calculate actual returns (R_it)
    std::vector<double> actualReturns = calculateReturns(prices);

    // Calculate market returns (R_mt)
    std::vector<double> marketReturnsVec = calculateReturns(marketPrices);

    // Calculate expected returns (alpha_i + beta_i * R_mt)
    std::vector<double> expectedReturns(prices.size(), 0.0);
    for (size_t i = 1; i < marketReturnsVec.size(); ++i) {
        expectedReturns[i] = stock.alpha + stock.beta * marketReturnsVec[i];
    }

    // Calculate abnormal returns (AR_it)
    std::vector<double> abnormalReturns(prices.size(), 0.0);
    for (size_t i = 1; i < prices.size(); ++i) {
        abnormalReturns[i] = actualReturns[i] - expectedReturns[i];
    }

    // Convert dates to numerical values for plotting
    std::vector<double> x_values(dates.size());
    for (size_t i = 0; i < dates.size(); ++i) {
        x_values[i] = static_cast<double>(i);
    }

    // Plot abnormal returns
    auto fig = matplot::figure(true);
    matplot::plot(x_values, abnormalReturns, "-o");

    // Add disclosure date marker if available
    if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        std::string target_date = stock.disclosureD;
        if (target_date.length() == 8) {
            target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
        }

        size_t disclosure_index = dates.size();
        for (size_t i = 0; i < dates.size(); ++i) {
            if (dates[i] == target_date) {
                disclosure_index = i;
                break;
            }
        }

        if (disclosure_index < dates.size()) {
            double x_disclosure = static_cast<double>(disclosure_index);
            double y_disclosure = abnormalReturns[disclosure_index];
            auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 0.01, x_disclosure, y_disclosure, "Disclosure Date");
            t->color("red").font_size(14);
            a->color("red");
        }
    }

    // Set plot title and labels
    std::string title = "Abnormal Returns: " + stock.tickerN;
    if (!stock.attackN.empty() && stock.attackN != "Unknown") {
        title += " (" + stock.attackN + ")";
    }
    if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        title += " - Disclosed: " + stock.disclosureD;
    }
    matplot::title(title);
    matplot::xlabel("Time Period");
    matplot::ylabel("Abnormal Return");
    matplot::grid(true);

    // Add date labels to the x-axis
    if (!dates.empty()) {
        matplot::xticks({0, static_cast<double>(dates.size() - 1)});
        matplot::xticklabels({dates[0], dates.back()});
    }

    // Save the plot
    matplot::save("../output/AbnormalReturn-" + stock.attackN + "-" + stock.tickerN, "svg");
}
