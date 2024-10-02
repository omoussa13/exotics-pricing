#include "Option.hpp"
#include "MonteCarlo.hpp"
#include "PayOff.hpp"
#include "StatFunctions.hpp"
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

std::ostream &operator<<(std::ostream &os, const OptionsParams &params) {
  os << "S0: " << params.S0 << ", E: " << params.E << ", T: " << params.T << ", sigma: " << params.sigma << ", r: " << params.r;

  return os;
}

std::ostream &operator<<(std::ostream &os, const ErrorData &err) {
  os << "Std error: " << err.standard_error << ", Confidence interval " << err.confidence << "%: [" << err.confidence_interval_low << " - "
     << err.confidence_interval_high << "]";

  return os;
}

Option::Option(std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params) : mc(mc), options_params(options_params) {}

Option::~Option() {}

FixedStrikeOption::FixedStrikeOption(std::unique_ptr<PayOffFixedStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                     OptionsParams &options_params)
    : Option(mc, options_params), pay_off(std::move(pay_off)) {}

FixedStrikeOption::~FixedStrikeOption() {}

FloatingStrikeOption::FloatingStrikeOption(std::unique_ptr<PayOffFloatingStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                           OptionsParams &options_params)
    : Option(mc, options_params), pay_off(std::move(pay_off)) {}

FloatingStrikeOption::~FloatingStrikeOption() {}

AsianFixedStrikeOption::AsianFixedStrikeOption(std::unique_ptr<PayOffFixedStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                               OptionsParams &options_params)
    : FixedStrikeOption(std::move(pay_off), mc, options_params) {}

AsianFixedStrikeOption::~AsianFixedStrikeOption() {}

AsianFloatingStrikeOption::AsianFloatingStrikeOption(std::unique_ptr<PayOffFloatingStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                                     OptionsParams &options_params)
    : FloatingStrikeOption(std::move(pay_off), mc, options_params) {}

AsianFloatingStrikeOption::~AsianFloatingStrikeOption() {}

LookbackFixedStrikeOption::LookbackFixedStrikeOption(std::unique_ptr<PayOffFixedStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                                     OptionsParams &options_params)
    : FixedStrikeOption(std::move(pay_off), mc, options_params) {}

LookbackFixedStrikeOption::~LookbackFixedStrikeOption() {}

LookbackFloatingStrikeOption::LookbackFloatingStrikeOption(std::unique_ptr<PayOffFloatingStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                                           OptionsParams &options_params)
    : FloatingStrikeOption(std::move(pay_off), mc, options_params) {}

LookbackFloatingStrikeOption::~LookbackFloatingStrikeOption() {}

double Option::discount(const double s) const {
  return std::exp(-this->options_params.r * this->options_params.T) * s;
}

double Option::std_error(const std::vector<double> &prices) const {
  if (prices.empty()) {
    throw std::invalid_argument("prices can't be empty");
  }
  double variance = compute_variance(prices);
  double avg = compute_average(prices);
  double discounted_var = std::exp(-2 * this->options_params.r * this->options_params.T) * variance;

  return std::sqrt(discounted_var / prices.size());
}

std::tuple<double, double, ErrorData, ErrorData> Option::compute_pricing_results(const std::vector<double> &pay_offs_call,
                                                                                 const std::vector<double> &pay_offs_put) const {
  double expected_pay_off_call = compute_average(pay_offs_call);
  double expected_pay_off_put = compute_average(pay_offs_put);
  double price_call = this->discount(expected_pay_off_call);
  double price_put = this->discount(expected_pay_off_put);
  double std_err_call = this->std_error(pay_offs_call);
  double std_err_put = this->std_error(pay_offs_put);
  double confidence_interval_low_call = price_call - CRITICAL_VALUE * std_err_call;
  double confidence_interval_low_put = price_put - CRITICAL_VALUE * std_err_put;
  double confidence_interval_high_call = price_call + CRITICAL_VALUE * std_err_call;
  double confidence_interval_high_put = price_put + CRITICAL_VALUE * std_err_put;
  ErrorData err_call = ErrorData{std_err_call, CONFIDENCE, confidence_interval_low_call, confidence_interval_high_call};
  ErrorData err_put = ErrorData{std_err_put, CONFIDENCE, confidence_interval_low_put, confidence_interval_high_put};

  return std::make_tuple(price_call, price_put, err_call, err_put);
}

std::tuple<double, double, ErrorData, ErrorData>
FixedStrikeOption::price_fixed_strike(int timesteps, int simulations, double (*stat_func_call)(const std::vector<double> &),
                                      double (*stat_func_put)(const std::vector<double> &),
                                      std::function<double(const double)> pay_off_func_call,
                                      std::function<double(const double)> pay_off_func_put) const {
  std::vector<double> prices(timesteps);
  std::vector<double> pay_offs_call;
  std::vector<double> pay_offs_put;

  for (int i = 0; i < simulations; ++i) {
    mc->simulate_price_path(prices, this->options_params.S0, this->options_params.r, this->options_params.sigma, this->options_params.T);
    double stat_call = stat_func_call(prices);
    double stat_put = stat_func_put(prices);
    double pay_off_call = pay_off_func_call(stat_call);
    double pay_off_put = pay_off_func_put(stat_put);

    pay_offs_call.push_back(pay_off_call);
    pay_offs_put.push_back(pay_off_put);
  }

  return compute_pricing_results(pay_offs_call, pay_offs_put);
}

std::tuple<double, double, ErrorData, ErrorData>
FloatingStrikeOption::price_floating_strike(int timesteps, int simulations, double (*stat_func_call)(const std::vector<double> &),
                                            double (*stat_func_put)(const std::vector<double> &),
                                            std::function<double(const double, const double)> pay_off_func_call,
                                            std::function<double(const double, const double)> pay_off_func_put) const {
  std::vector<double> prices(timesteps);
  std::vector<double> pay_offs_call;
  std::vector<double> pay_offs_put;

  for (int i = 0; i < simulations; ++i) {
    mc->simulate_price_path(prices, this->options_params.S0, this->options_params.r, this->options_params.sigma, this->options_params.T);
    double stat_call = stat_func_call(prices);
    double stat_put = stat_func_put(prices);
    double s = prices.back();
    double pay_off_call = pay_off_func_call(stat_call, s);
    double pay_off_put = pay_off_func_put(stat_put, s);

    pay_offs_call.push_back(pay_off_call);
    pay_offs_put.push_back(pay_off_put);
  }

  return compute_pricing_results(pay_offs_call, pay_offs_put);
}

std::tuple<double, double, ErrorData, ErrorData> AsianFixedStrikeOption::operator()(int timesteps, int simulations) const {
  return this->price_fixed_strike(
      timesteps, simulations, &compute_average, &compute_average,
      [this](double avg) -> double {
        return this->pay_off->call(avg);
      },
      [this](double avg) -> double {
        return this->pay_off->put(avg);
      });
}

std::tuple<double, double, ErrorData, ErrorData> AsianFloatingStrikeOption::operator()(int timesteps, int simulations) const {
  return this->price_floating_strike(
      timesteps, simulations, &compute_average, &compute_average,
      [this](double avg, double s) -> double {
        return this->pay_off->call(avg, s);
      },
      [this](double avg, double s) -> double {
        return this->pay_off->put(avg, s);
      });
}

std::tuple<double, double, ErrorData, ErrorData> LookbackFixedStrikeOption::operator()(int timesteps, int simulations) const {
  return this->price_fixed_strike(
      timesteps, simulations, &find_max, &find_min,
      [this](double max) -> double {
        return this->pay_off->call(max);
      },
      [this](double min) -> double {
        return this->pay_off->put(min);
      });
}

std::tuple<double, double, ErrorData, ErrorData> LookbackFloatingStrikeOption::operator()(int timesteps, int simulations) const {
  return this->price_floating_strike(
      timesteps, simulations, &find_min, &find_max,
      [this](double min, double s) -> double {
        return this->pay_off->call(min, s);
      },
      [this](double max, double s) -> double {
        return this->pay_off->put(max, s);
      });
}