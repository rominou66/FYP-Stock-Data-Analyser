#include "../include/plotter.hpp"

/**
 * @brief Plot individual graph of a single stock, with disclosure date annotation and output an SVG
 *
 * @param stock
 * @param before Number of days before disclosure date to plot
 * @param after Number of days after disclosure date to plot
 */
void singlePlot(Stock stock, int before, int after) {
    if(stock.histD.empty() || stock.histD[0].dates.empty() || stock.histD[0].prices.empty()) {
        std::cerr << "No data available to plot for stock: " << stock.tickerN << std::endl;
        return;
    }

    // Extract data from the first historical data set
    const auto& dates = stock.histD[0].dates;
    const auto& prices = stock.histD[0].prices;

    // Find disclosure date index
    size_t disclosure_index = dates.size();
    if(!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        std::string target_date = stock.disclosureD;
        if(target_date.length() == 8) {
            target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
        }
        for(size_t i = 0; i < dates.size(); i++) {
            if(dates[i] == target_date) {
                disclosure_index = i;
                break;
            }
        }
    }

    // Calculate start and end indices
    size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
    size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

    // Slice data for plotting
    std::vector<double> x_values, prices_sliced;
    std::vector<std::string> dates_sliced;
    for(size_t i = start_index; i <= end_index; ++i) {
        x_values.push_back(static_cast<double>(i - start_index)); // Reset x to start at 0
        prices_sliced.push_back(prices[i]);
        dates_sliced.push_back(dates[i]);
    }

    // Create the plot
    auto fig = matplot::figure(true);
    matplot::plot(x_values, prices_sliced, "-o");

    // Add disclosure date marker
    if(disclosure_index < dates.size()) {
        double x_disclosure = static_cast<double>(disclosure_index - start_index);
        double y_disclosure = prices_sliced[disclosure_index - start_index];
        auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 2.5, x_disclosure, y_disclosure, "Disclosure Date");
        t->color("red").font_size(14);
        a->color("red");
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
    matplot::gca()->minor_grid(true);

    // Force x-ticks and x-tick labels at regular intervals
    std::vector<double> xticks;
    std::vector<std::string> xticklabels;
    for(size_t i = 0; i < dates_sliced.size(); i += 4) {
        xticks.push_back(static_cast<double>(i));
        std::string abbreviated_date = dates_sliced[i].substr(0, 5);
        xticklabels.push_back(abbreviated_date);
    }

    matplot::xticks(xticks);
    matplot::xticklabels(xticklabels);
    matplot::xtickangle(45);

    // Save the plot with 'before' and 'after' in the filename
    std::string filename = "../output/" + stock.attackN + "-" + stock.tickerN +
                           "-before" + std::to_string(before) +
                           "-after" + std::to_string(after);
    matplot::save(filename + ".svg");
    savePlottedDataToCSV(filename + ".csv", dates_sliced, prices_sliced, stock.tickerN, before, after);
}

/**
 * @brief Plot all stocks in the vector on a single graph, with disclosure date annotations and output an SVG
 *
 * @param stocks Vector of Stock objects to plot
 * @param before Number of days before disclosure date to plot
 * @param after Number of days after disclosure date to plot
 */
void multiPlot(const std::vector<Stock>& stocks, int before, int after) {
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

    std::vector<std::string> stock_names;
    std::vector<std::vector<std::string>> dates_list;
    std::vector<std::vector<double>> y_values_list;

// Populate stock_names, dates_list, and y_values_list for each stock
    for (const auto& stock : stocks) {
        stock_names.push_back(stock.tickerN);
        dates_list.push_back(stock.histD[0].dates);      // Your dates for this stock
        y_values_list.push_back(stock.histD[0].prices); // Your y values for this stock
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

        // Find disclosure date index
        size_t disclosure_index = dates.size();
        if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
            std::string target_date = stock.disclosureD;
            if (target_date.length() == 8) {
                target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
            }
            for (size_t i = 0; i < dates.size(); i++) {
                if (dates[i] == target_date) {
                    disclosure_index = i;
                    break;
                }
            }
        }

        // Calculate start and end indices
        size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
        size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

        // Slice data for plotting
        std::vector<double> x_values;
        std::vector<double> prices_sliced;
        for (size_t i = start_index; i <= end_index; ++i) {
            x_values.push_back(i - start_index);
            prices_sliced.push_back(prices[i]);
        }

        // Plot the stock data
        matplot::plot(x_values, prices_sliced, "-o")->display_name(stock.tickerN);

        // Add disclosure date marker
        // if (disclosure_index < dates.size()) {
        //     double x_disclosure = static_cast<double>(disclosure_index - start_index);
        //     double y_disclosure = prices_sliced[disclosure_index - start_index];
        //     auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 2.5, x_disclosure, y_disclosure, "Disclosure: " + stock.tickerN);
        //     t->color("red").font_size(10);
        //     a->color("red");
        // }
    }

    // Set plot title and labels
    std::string title = "Historical Stock Prices (Before: " + std::to_string(before) + " After: " + std::to_string(after) + ")";
    matplot::title(title);
    matplot::xlabel("Date");
    matplot::ylabel("Price ($)");
    matplot::grid(true);
    matplot::legend();

    // Set x-ticks and labels using dates from the first stock (abbreviated)
    if (!stocks[0].histD.empty() && !stocks[0].histD[0].dates.empty()) {
        const auto& dates = stocks[0].histD[0].dates;
        size_t disclosure_index = dates.size();
        if (!stocks[0].disclosureD.empty() && stocks[0].disclosureD != "Unknown") {
            std::string target_date = stocks[0].disclosureD;
            if (target_date.length() == 8) {
                target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
            }
            for (size_t i = 0; i < dates.size(); i++) {
                if (dates[i] == target_date) {
                    disclosure_index = i;
                    break;
                }
            }
        }
        size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
        size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

        // Use every 2th date for x-ticks (abbreviated)
        std::vector<double> xticks;
        std::vector<std::string> xticklabels;
        for (size_t i = start_index; i <= end_index; i += 2) {
            xticks.push_back(i - start_index);
            std::string abbreviated_date = dates[i].substr(0, 5); // "dd/mm/yyyy" -> "dd/mm"
            xticklabels.push_back(abbreviated_date);
        }
        matplot::xticks(xticks);
        matplot::xticklabels(xticklabels);
        matplot::xtickangle(45);
    }

    // Save the plot
    std::string filename = "../output/multi-stock-plot-before" + std::to_string(before) + "-after" + std::to_string(after);
    matplot::save(filename + ".svg");
    savePlottedDataToCSV(filename + ".csv", stock_names, dates_list, y_values_list, before, after);
    matplot::hold(matplot::off);
}

/**
 * @brief Plot all stocks in the vector on a single graph (normalized), with disclosure date annotations and output an SVG
 *
 * @param stocks Vector of Stock objects to plot
 * @param before Number of days before disclosure date to plot
 * @param after Number of days after disclosure date to plot
 */
void normalizedMultiPlot(const std::vector<Stock>& stocks, int before, int after) {
    if (stocks.empty()) {
        std::cerr << "No stocks provided to plot." << std::endl;
        return;
    }

    std::vector<std::string> stock_names;
    std::vector<std::vector<std::string>> dates_list;
    std::vector<std::vector<double>> y_values_list;

// Populate stock_names, dates_list, and y_values_list for each stock
    for (const auto& stock : stocks) {
        stock_names.push_back(stock.tickerN);
        dates_list.push_back(stock.histD[0].dates);      // Your dates for this stock
        y_values_list.push_back(stock.histD[0].prices); // Your y values for this stock
    }

    // Create the plot
    auto fig = matplot::figure(true);
    matplot::hold(matplot::on);

    // Track if any valid stock was plotted
    bool has_plotted = false;

    // Plot each stock's data, normalized
    for (const auto& stock : stocks) {
        if (stock.histD.empty() || stock.histD[0].dates.empty() || stock.histD[0].prices.empty()) {
            std::cerr << "No data available to plot for stock: " << stock.tickerN << std::endl;
            continue;
        }

        const auto& dates = stock.histD[0].dates;
        const auto& prices = stock.histD[0].prices;

        // Find disclosure date index
        size_t disclosure_index = dates.size();
        if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
            std::string target_date = stock.disclosureD;
            if (target_date.length() == 8) {
                target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
            }
            for (size_t i = 0; i < dates.size(); i++) {
                if (dates[i] == target_date) {
                    disclosure_index = i;
                    break;
                }
            }
        }

        // Calculate start and end indices
        size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
        size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

        // Skip if no valid range
        if (start_index >= dates.size() || end_index < start_index) {
            std::cerr << "Invalid date range for stock: " << stock.tickerN << ". Skipping..." << std::endl;
            continue;
        }

        // Slice data for plotting
        std::vector<double> x_values;
        std::vector<double> prices_sliced;
        for (size_t i = start_index; i <= end_index; ++i) {
            x_values.push_back(i - start_index);
            prices_sliced.push_back(prices[i]);
        }

        // Skip if no prices in the sliced range
        if (prices_sliced.empty()) {
            std::cerr << "No prices in the sliced range for stock: " << stock.tickerN << ". Skipping..." << std::endl;
            continue;
        }

        // Normalize prices to [0, 1] range
        double min_price = *std::min_element(prices_sliced.begin(), prices_sliced.end());
        double max_price = *std::max_element(prices_sliced.begin(), prices_sliced.end());

        // Avoid division by zero
        if (max_price == min_price) {
            std::cerr << "All prices are the same for stock: " << stock.tickerN << ". Skipping..." << std::endl;
            continue;
        }

        std::vector<double> normalized_prices(prices_sliced.size());
        for (size_t i = 0; i < prices_sliced.size(); i++) {
            normalized_prices[i] = (prices_sliced[i] - min_price) / (max_price - min_price);
        }

        // Plot the normalized stock data
        matplot::plot(x_values, normalized_prices, "-o")->display_name(stock.tickerN);
        has_plotted = true;

        // Add disclosure date marker
        // if (disclosure_index < dates.size()) {
        //     double x_disclosure = static_cast<double>(disclosure_index - start_index);
        //     double y_disclosure = normalized_prices[disclosure_index - start_index];
        //     auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 0.1, x_disclosure, y_disclosure, "Disclosure: " + stock.tickerN);
        //     t->color("red").font_size(10);
        //     a->color("red");
        // }
    }

    // If no valid stocks were plotted, exit
    if (!has_plotted) {
        std::cerr << "No valid stocks to plot." << std::endl;
        return;
    }

    // Set plot title and labels
    std::string title = "Normalized Historical Stock Price Movements (Before: " + std::to_string(before) + " After: " + std::to_string(after) + ")";
    matplot::title(title);
    matplot::xlabel("Date");
    matplot::ylabel("Normalized Price");
    matplot::grid(true);
    matplot::legend();
    matplot::gca()->minor_grid(true);

    // Use the first valid stock's dates for x-ticks (abbreviated)
    for (const auto& stock : stocks) {
        if (!stock.histD.empty() && !stock.histD[0].dates.empty() && !stock.histD[0].prices.empty()) {
            const auto& dates = stock.histD[0].dates;
            size_t disclosure_index = dates.size();
            if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
                std::string target_date = stock.disclosureD;
                if (target_date.length() == 8) {
                    target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
                }
                for (size_t i = 0; i < dates.size(); i++) {
                    if (dates[i] == target_date) {
                        disclosure_index = i;
                        break;
                    }
                }
            }
            size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
            size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

            // Use every 4th date for x-ticks (abbreviated)
            std::vector<double> xticks;
            std::vector<std::string> xticklabels;
            for (size_t i = start_index; i <= end_index; i += 4) {
                xticks.push_back(i - start_index);
                std::string abbreviated_date = dates[i].substr(0, 5); // "dd/mm/yyyy" -> "dd/mm"
                xticklabels.push_back(abbreviated_date);
            }
            matplot::xticks(xticks);
            matplot::xticklabels(xticklabels);
            matplot::xtickangle(45);
            break; // Only use the first valid stock for x-ticks
        }
    }

    // Save the plot
    std::string filename = "../output/normalized-multi-stock-plot-before" + std::to_string(before) + "-after" + std::to_string(after) + ".svg";
    matplot::save(filename + ".svg");
    savePlottedDataToCSV(filename + ".csv", stock_names, dates_list, y_values_list, before, after);
    matplot::hold(matplot::off);
}

/**
 * @brief Single plot for Abnormal Return
 *
 * @param stock
 * @param marketReturns
 * @param before Number of days before disclosure date to plot
 * @param after Number of days after disclosure date to plot
 */
void abnormalReturnPlot(Stock stock, Stock marketReturns, int before, int after) {
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

    // Find disclosure date index
    size_t disclosure_index = dates.size();
    if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        std::string target_date = stock.disclosureD;
        if (target_date.length() == 8) {
            target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
        }
        for (size_t i = 0; i < dates.size(); ++i) {
            if (dates[i] == target_date) {
                disclosure_index = i;
                break;
            }
        }
    }

    // Calculate start and end indices
    size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
    size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

    // Slice data for plotting
    std::vector<double> x_values, abnormalReturns_sliced;
    std::vector<std::string> dates_sliced;
    for (size_t i = start_index; i <= end_index; ++i) {
        x_values.push_back(static_cast<double>(i - start_index)); // Reset x to start at 0
        abnormalReturns_sliced.push_back(abnormalReturns[i]);
        dates_sliced.push_back(dates[i]);
    }

    // Plot abnormal returns
    auto fig = matplot::figure(true);
    matplot::plot(x_values, abnormalReturns_sliced, "-o");

    // Add disclosure date marker
    if( before > 0) {
        if (disclosure_index < dates.size()) {
            double x_disclosure = static_cast<double>(disclosure_index - start_index);
            double y_disclosure = abnormalReturns_sliced[disclosure_index - start_index];
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
    matplot::gca()->minor_grid(true);

    // Force x-ticks and x-tick labels at regular intervals
    std::vector<double> xticks;
    std::vector<std::string> xticklabels;
    for (size_t i = 0; i < dates_sliced.size(); i += 4) {
        xticks.push_back(static_cast<double>(i));
        std::string abbreviated_date = dates_sliced[i].substr(0, 5);
        xticklabels.push_back(abbreviated_date);
    }

    matplot::xticks(xticks);
    matplot::xticklabels(xticklabels);
    matplot::xtickangle(45);

    // Save the plot with 'before' and 'after' in the filename
    std::string filename = "../output/AbnormalReturn-" + stock.attackN + "-" + stock.tickerN +
                           "-before" + std::to_string(before) +
                           "-after" + std::to_string(after);
    matplot::save(filename + ".svg");
    savePlottedDataToCSV(filename + ".csv", dates_sliced, abnormalReturns_sliced, stock.tickerN, before, after);
}



/**
* @brief Single plot for Cumulative Abnormal Return
*
* @param stock
* @param marketReturns
* @param before Number of days before disclosure date to plot
* @param after Number of days after disclosure date to plot
*/
void cumulativeARPlot(Stock stock, Stock marketReturns, int before, int after) {
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

    // Calculate cumulative abnormal returns (CAR)
    std::vector<double> car = calculateCAR(abnormalReturns);

    // Find disclosure date index
    size_t disclosure_index = dates.size();
    if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        std::string target_date = stock.disclosureD;
        if (target_date.length() == 8) {
            target_date = target_date.substr(0, 2) + "/" + target_date.substr(2, 2) + "/" + target_date.substr(4, 4);
        }
        for (size_t i = 0; i < dates.size(); ++i) {
            if (dates[i] == target_date) {
                disclosure_index = i;
                break;
            }
        }
    }

// Calculate start and end indices
    size_t start_index = (disclosure_index > before) ? disclosure_index - before : 0;
    size_t end_index = (disclosure_index + after < dates.size()) ? disclosure_index + after : dates.size() - 1;

// Slice data for plotting
    std::vector<double> x_values, car_sliced;
    std::vector<std::string> dates_sliced;
    for (size_t i = start_index; i <= end_index; ++i) {
        x_values.push_back(static_cast<double>(i - start_index)); // Reset x to start at 0
        car_sliced.push_back(car[i]);
        dates_sliced.push_back(dates[i]);
    }

// Plot CAR
    auto fig = matplot::figure(true);
    matplot::plot(x_values, car_sliced, "-o");

// Add disclosure date marker
    if(before > 0) {
            if (disclosure_index < dates.size()) {
                double x_disclosure = static_cast<double>(disclosure_index - start_index);
                double y_disclosure = car_sliced[disclosure_index - start_index];
                auto [t, a] = matplot::textarrow(x_disclosure + 2.5, y_disclosure + 0.01, x_disclosure, y_disclosure, "Disclosure Date");
                t->color("red").font_size(14);
                a->color("red");
            }
    }

// Set plot title and labels
    std::string title = "Cumulative Abnormal Returns (CAR): " + stock.tickerN;
    if (!stock.attackN.empty() && stock.attackN != "Unknown") {
        title += " (" + stock.attackN + ")";
    }
    if (!stock.disclosureD.empty() && stock.disclosureD != "Unknown") {
        title += " - Disclosed: " + stock.disclosureD;
    }
    matplot::title(title);
    matplot::xlabel("Time Period");
    matplot::ylabel("Cumulative Abnormal Return (CAR)");
    matplot::grid(true);
    matplot::gca()->minor_grid(true);

// Force x-ticks and x-tick labels at regular intervals
    std::vector<double> xticks;
    std::vector<std::string> xticklabels;
    for (size_t i = 0; i < dates_sliced.size(); i += 4) {
        xticks.push_back(static_cast<double>(i));
        std::string abbreviated_date = dates_sliced[i].substr(0, 5);
        xticklabels.push_back(abbreviated_date);
    }

    matplot::xticks(xticks);
    matplot::xticklabels(xticklabels);
    matplot::xtickangle(45);

// Save the plot with before and after in the filename
    std::string filename = "../output/CAR-" + stock.attackN + "-" + stock.tickerN +
                           "-before" + std::to_string(before) +
                           "-after" + std::to_string(after);
    matplot::save(filename + ".svg");
    savePlottedDataToCSV(filename + ".csv", dates_sliced, car_sliced, stock.tickerN, before, after);
}

