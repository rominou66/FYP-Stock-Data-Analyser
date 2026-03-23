#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <matplot/matplot.h>

#include "stock.hpp"
#include "analysis.hpp"

void singlePlot(Stock stock, int before, int after);
void multiPlot(const std::vector<Stock>& stocks, int before, int after);
void normalizedMultiPlot(const std::vector<Stock>& stocks, int before, int after);
void abnormalReturnPlot(Stock stock, Stock marketReturns, int before, int after);
void cumulativeARPlot(Stock stock, Stock marketReturns, int before, int after);

#endif //PLOTTER_HPP
