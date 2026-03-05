#include "../include/plotter.hpp"

void plot(std::vector<std::string> dates, std::vector<double> prices) {
    matplot::plot(prices, "-o");
    matplot::title("Historical Stock Price");
    matplot::xlabel("Date");
    matplot::ylabel("Price ($)");
    matplot::grid(true);
    matplot::show();
}
