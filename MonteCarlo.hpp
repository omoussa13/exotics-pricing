#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <cmath>
#include <random>
#include <vector>

class MonteCarlo {
  public:
    MonteCarlo();
    ~MonteCarlo();

    double generate_random_number();
    void simulate_price_path(std::vector<double> &prices, const double &S0, const double &r, const double &sigma, const double &T);

  private:
    std::mt19937 rd_generator;
};

#endif
