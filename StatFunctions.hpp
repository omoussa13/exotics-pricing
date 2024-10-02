#ifndef STATFUNCTIONS_H
#define STATFUNCTIONS_H

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>

inline double compute_average(const std::vector<double> &prices) {
  if (prices.empty()) {
    throw std::invalid_argument("list is empty");
  }

  double sum = std::accumulate(prices.begin(), prices.end(), 0.0);
  return sum / prices.size();
}

inline double compute_variance(const std::vector<double> &prices) {
  if (prices.empty()) {
    throw std::invalid_argument("list empty");
  }

  double avg = compute_average(prices);
  double sum{0.0};
  for (double price : prices) {
    sum += (price - avg) * (price - avg);
  }
  return sum / prices.size();
}

inline double find_max(const std::vector<double> &prices) {
  if (prices.empty()) {
    throw std::invalid_argument("list is empty");
  }

  return *std::max_element(prices.begin(), prices.end());
}

inline double find_min(const std::vector<double> &prices) {
  if (prices.empty()) {
    throw std::invalid_argument("list is empty");
  }

  return *std::min_element(prices.begin(), prices.end());
}

#endif