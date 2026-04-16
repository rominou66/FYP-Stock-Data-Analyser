#include "../include/parser.hpp"

void builder() {
    Stock stock;
    Hist_Data histDat;
}

/**
 * @brief Read a CSV and return a Stock struct containing all the parse data
 *
 * @param path
 * @param name
 * @return Stock
 */
Stock parseCSV(const std::string& path, const std::string& name) {
    std::ifstream csvFile(path);
    if(!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << path << std::endl;
        return Stock();
    }

    Stock stock;
    Hist_Data histData;

    // Parse filename format: attackN-tickerN-ddmmyyyy or attackN-tickerN
    size_t last_dash_pos = name.rfind('-');
    size_t first_dash_pos = name.find('-');

    if(first_dash_pos != std::string::npos) {
        stock.attackN = name.substr(0, first_dash_pos);

        if(last_dash_pos != first_dash_pos && last_dash_pos != std::string::npos) {
            // Format: attackN-tickerN-ddmmyyyy
            stock.tickerN = name.substr(first_dash_pos + 1, last_dash_pos - first_dash_pos - 1);
            stock.disclosureD = name.substr(last_dash_pos + 1); // ddmmyyyy format
        } else {
            // Format: attackN-tickerN
            stock.tickerN = name.substr(first_dash_pos + 1);
            stock.disclosureD = "Unknown";
        }
    } else {
        // Fallback: if no dash, use whole name as ticker
        stock.tickerN = name;
        stock.attackN = "Unknown";
        stock.disclosureD = "Unknown";
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(csvFile, line)) {
        if(isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string date, priceStr;
        std::getline(ss, date, ',');
        std::getline(ss, priceStr, ',');

        try {
            double price = std::stod(priceStr);
            histData.dates.push_back(date);
            histData.prices.push_back(price);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing price: " << priceStr << std::endl;
        }
    }

    stock.histD.push_back(histData);
    csvFile.close();
    return stock;
}

/**
 * @brief Return all parse data from the CSVs in a vector of Stocks
 *
 * @return std::vector<Stock>
 */
std::vector<Stock> processAllCSVFiles() {
    std::vector<Stock> stocks;
    try {
        // Try relative path first
        std::filesystem::path csvDir = "data_csv";

        // If that fails, try with parent directory (for when running from build/)
        if(!std::filesystem::exists(csvDir)) {
            csvDir = "../data_csv";
        }

        for(const auto& entry : std::filesystem::directory_iterator(csvDir)) {
            if(entry.path().extension() == ".csv") {
                std::string name = entry.path().stem().string();
                Stock stock = parseCSV(entry.path().string(), name);
                stocks.push_back(stock);
                std::cout << "Processed: " << name << ".csv with " << stock.histD[0].dates.size() << " data points" << std::endl;
                std::cout << "Vulnaribility: " + stock.attackN << std::endl;
                std::cout << "Ticker: " + stock.tickerN << std::endl;
                std::cout << "Disclosure: " + stock.disclosureD << std::endl << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing CSV files: " << e.what() << std::endl;
    }
    return stocks;
}

/**
* @brief Save plotted dates and y values to a CSV file for a single stock
*
* @param filename Name of the CSV file to save
* @param dates Vector of date strings (e.g., "dd/mm/yyyy")
* @param y_values Vector of y values (e.g., prices or normalized prices)
* @param stock_name Name of the stock (for CSV header)
* @param before Number of days before disclosure date (for CSV header)
* @param after Number of days after disclosure date (for CSV header)
*/
void savePlottedDataToCSV(
    const std::string& filename,
    const std::vector<std::string>& dates,
    const std::vector<double>& y_values,
    const std::string& stock_name,
    int before,
    int after
) {
// Open the CSV file for writing
    std::ofstream csv_file(filename);
    if (!csv_file.is_open()) {
        std::cerr << "Failed to open CSV file: " << filename << std::endl;
        return;
    }

// Write CSV header
    csv_file << "Stock,Before,After,Date,Value\n";
    csv_file << stock_name << "," << before << "," << after << ",,\n"; // Metadata row

// Write dates and y values
    for (size_t i = 0; i < dates.size(); ++i) {
        if (i < y_values.size()) {
            csv_file << "," << "," << "," << dates[i] << "," << y_values[i] << "\n";
        }
    }

    csv_file.close();
    std::cout << "Plotted data saved to: " << filename << std::endl;
}


/**
* @brief Save plotted dates and y values for multiple stocks to a single CSV file
*
* @param filename Name of the CSV file to save
* @param stock_names Vector of stock names
* @param dates_list Vector of vectors of date strings (one per stock)
* @param y_values_list Vector of vectors of y values (one per stock)
* @param before Number of days before disclosure date (for CSV header)
* @param after Number of days after disclosure date (for CSV header)
*/
void savePlottedDataToCSV(
    const std::string& filename,
    const std::vector<std::string>& stock_names,
    const std::vector<std::vector<std::string>>& dates_list,
    const std::vector<std::vector<double>>& y_values_list,
    int before,
    int after
) {
// Open the CSV file for writing
    std::ofstream csv_file(filename);
    if (!csv_file.is_open()) {
        std::cerr << "Failed to open CSV file: " << filename << std::endl;
        return;
    }

// Write CSV header
    csv_file << "Before,After,";
    for (size_t i = 0; i < stock_names.size(); ++i) {
        csv_file << stock_names[i] << "_Date," << stock_names[i] << "_Value,";
    }
    csv_file << "\n";

// Write metadata row
    csv_file << before << "," << after << ",";
    for (size_t i = 0; i < stock_names.size(); ++i) {
        csv_file << ",";
    }
    csv_file << "\n";

// Determine the maximum number of data points
    size_t max_points = 0;
    for (size_t i = 0; i < dates_list.size(); ++i) {
        if (dates_list[i].size() > max_points) {
            max_points = dates_list[i].size();
        }
    }

// Write data rows
    for (size_t i = 0; i < max_points; ++i) {
        csv_file << "," << ",";
        for (size_t j = 0; j < stock_names.size(); ++j) {
            if (i < dates_list[j].size() && i < y_values_list[j].size()) {
                csv_file << dates_list[j][i] << "," << y_values_list[j][i] << ",";
            } else {
                csv_file << ",,";
            }
        }
        csv_file << "\n";
    }

    csv_file.close();
    std::cout << "Plotted data saved to: " << filename << std::endl;
}


