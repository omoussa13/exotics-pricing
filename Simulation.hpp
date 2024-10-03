#ifndef SIMULATION_H
#define SIMULATION_H

#include "MonteCarlo.hpp"
#include "Option.hpp"
#include <memory>
#include <ostream>
#include <string>

class Simulation {
  public:
    Simulation(std::string name, int timesteps, int simulations, std::shared_ptr<MonteCarlo> mc, OptionsParams params);
    ~Simulation();

    void change_S0(double S0);
    void change_T(double T);
    void change_E(double E);
    void change_sigma(double sigma);
    void change_r(double r);
    void reset_params(const OptionsParams &params);

    friend std::ostream &operator<<(std::ostream &os, const Simulation &simulation);
    void fill_price_data(const std::tuple<double, double, ErrorData, ErrorData> &result);
    void simulate_asian_option_fixed_strike();
    void simulate_asian_option_floating_strike();
    void simulate_lookback_option_fixed_strike();
    void simulate_lookback_option_floating_strike();

  private:
    std::string name;
    OptionsParams option_params;
    int timesteps;
    int nb_simulations;
    double price_call;
    double price_put;
    ErrorData err_call;
    ErrorData err_put;
    std::shared_ptr<MonteCarlo> mc;
};

#endif