#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>

#include "stock.hpp"

std::vector<double> calculateReturns(const std::vector<double>& prices);
void estimateAlphaBeta(Stock& stockReturns, Stock& marketReturns);

#endif //ANALYSIS_HPP
