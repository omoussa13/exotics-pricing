#include "MonteCarlo.hpp"
#include <random>

MonteCarlo::MonteCarlo() {
  std::mt19937 g(987654321);
  this->rd_generator = g;
}

MonteCarlo::~MonteCarlo() {}

double MonteCarlo::generate_random_number() {
  std::normal_distribution<double> normal_dist{0.0, 1.0};
  return normal_dist(rd_generator);
}

void MonteCarlo::simulate_price_path(std::vector<double> &prices, const double &S0, const double &r, const double &sigma, const double &T) {
  double timesteps = prices.size();
  double dt = T / timesteps;
  prices[0] = S0;

  for (int i = 1; i < timesteps; ++i) {
    double w = generate_random_number();
    prices[i] = prices[i - 1] * std::exp(((r - 0.5 * sigma * sigma) * dt) + sigma * std::sqrt(dt) * w);
  }
}