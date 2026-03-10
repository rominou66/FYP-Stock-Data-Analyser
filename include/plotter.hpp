#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <matplot/matplot.h>

#include "stock.hpp"

void singlePlot(Stock stock);
void multiPlot(const std::vector<Stock>& stocks);
void normalizedMultiPlot(const std::vector<Stock>& stocks);

#endif //PLOTTER_HPP
