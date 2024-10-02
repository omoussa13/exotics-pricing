#include "Simulation.hpp"
#include "MonteCarlo.hpp"
#include "Option.hpp"
#include <iostream>
#include <memory>
#include <tuple>

Simulation::Simulation(std::string name, int timesteps, int simulations, std::shared_ptr<MonteCarlo> mc, OptionsParams params)
    : name(name), timesteps(timesteps), nb_simulations(simulations), price_call(0.0), price_put(0.0), option_params(params), mc(mc) {}

Simulation::~Simulation() {}

std::ostream &operator<<(std::ostream &os, const Simulation &simulation) {
  os << "================= " << simulation.name << " =================" << std::endl;
  os << simulation.option_params << ", simulations: " << simulation.nb_simulations << ", timesteps: " << simulation.timesteps << std::endl;
  os << "CALL: Price: " << simulation.price_call << ", " << simulation.err_call << std::endl;
  os << "PUT: Price: " << simulation.price_put << ", " << simulation.err_put << std::endl;

  return os;
}

void Simulation::change_S0(double S0) {
  option_params.S0 = S0;
}

void Simulation::change_E(double E) {
  option_params.E = E;
}

void Simulation::change_T(double T) {
  option_params.T = T;
}

void Simulation::change_sigma(double sigma) {
  option_params.sigma = sigma;
}

void Simulation::change_r(double r) {
  option_params.r = r;
}

void Simulation::fill_price_data(const std::tuple<double, double, ErrorData, ErrorData> &result) {
  price_call = std::get<0>(result);
  price_put = std::get<1>(result);
  err_call = std::get<2>(result);
  err_put = std::get<3>(result);
}

void Simulation::reset_params(const OptionsParams &params) {
  this->option_params.S0 = params.S0;
  this->option_params.E = params.E;
  this->option_params.T = params.T;
  this->option_params.sigma = params.sigma;
  this->option_params.r = params.r;
}

void Simulation::simulate_asian_option_fixed_strike() {
  std::unique_ptr<PayOffFixedStrike> asian_fixed_strike_pay_off = std::make_unique<AsianFixedStrikePayOff>(option_params.E);
  std::unique_ptr<Option> asian_fixed_strike_option =
      std::make_unique<AsianFixedStrikeOption>(std::move(asian_fixed_strike_pay_off), mc, option_params);

  std::tuple<double, double, ErrorData, ErrorData> prices = (*asian_fixed_strike_option)(timesteps, nb_simulations);
  fill_price_data(prices);
}

void Simulation::simulate_asian_option_floating_strike() {
  std::unique_ptr<PayOffFloatingStrike> asian_floating_strike_pay_off = std::make_unique<AsianFloatingStrikePayOff>();
  std::unique_ptr<Option> asian_floating_strike_option =
      std::make_unique<AsianFloatingStrikeOption>(std::move(asian_floating_strike_pay_off), mc, option_params);

  std::tuple<double, double, ErrorData, ErrorData> prices = (*asian_floating_strike_option)(timesteps, nb_simulations);
  fill_price_data(prices);
}

void Simulation::simulate_lookback_option_fixed_strike() {
  std::unique_ptr<PayOffFixedStrike> lookback_fixed_strike_pay_off = std::make_unique<LookbackFixedStrikePayOff>(option_params.E);
  std::unique_ptr<Option> lookback_fixed_strike_option =
      std::make_unique<LookbackFixedStrikeOption>(std::move(lookback_fixed_strike_pay_off), mc, option_params);

  std::tuple<double, double, ErrorData, ErrorData> prices = (*lookback_fixed_strike_option)(timesteps, nb_simulations);
  fill_price_data(prices);
}

void Simulation::simulate_lookback_option_floating_strike() {
  std::unique_ptr<PayOffFloatingStrike> lookback_floating_strike_pay_off = std::make_unique<LookbackFloatingStrikePayOff>();
  std::unique_ptr<Option> lookback_floating_strike_option =
      std::make_unique<LookbackFloatingStrikeOption>(std::move(lookback_floating_strike_pay_off), mc, option_params);

  std::tuple<double, double, ErrorData, ErrorData> prices = (*lookback_floating_strike_option)(timesteps, nb_simulations);
  fill_price_data(prices);
}