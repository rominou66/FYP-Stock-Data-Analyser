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
