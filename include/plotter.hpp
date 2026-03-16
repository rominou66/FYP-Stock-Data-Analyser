#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <matplot/matplot.h>

#include "stock.hpp"
#include "analysis.hpp"

void singlePlot(Stock stock);
void multiPlot(const std::vector<Stock>& stocks);
void normalizedMultiPlot(const std::vector<Stock>& stocks);
void abnormalReturnPlot(Stock stock, Stock marketreturns);

#endif //PLOTTER_HPP
