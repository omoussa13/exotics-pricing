#ifndef OPTION_H
#define OPTION_H

#include "MonteCarlo.hpp"
#include "PayOff.hpp"
#include <functional>
#include <memory>
#include <vector>

const double CRITICAL_VALUE = 1.96;
const int CONFIDENCE = 95;

struct ErrorData {
    double standard_error;
    int confidence;
    double confidence_interval_low;
    double confidence_interval_high;
};

std::ostream &operator<<(std::ostream &os, const ErrorData &err);

struct OptionsParams {
    double S0;
    double E;
    double T;
    double sigma;
    double r;
};

std::ostream &operator<<(std::ostream &os, const OptionsParams &params);

class Option {
  public:
    Option(std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params);
    virtual ~Option();

    virtual std::tuple<double, double, ErrorData, ErrorData> operator()(int timesteps, int simulations) const = 0;
    virtual double discount(const double s) const;
    virtual double std_error(const std::vector<double> &prices) const;

  protected:
    std::shared_ptr<MonteCarlo> mc;
    OptionsParams &options_params;
    virtual std::tuple<double, double, ErrorData, ErrorData> compute_pricing_results(const std::vector<double> &pay_offs_call,
                                                                                     const std::vector<double> &pay_offs_put) const;
};

class FixedStrikeOption : public Option {
  public:
    FixedStrikeOption(std::unique_ptr<PayOffFixedStrike> pay_off, std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params);
    virtual ~FixedStrikeOption();

    virtual std::tuple<double, double, ErrorData, ErrorData> price_fixed_strike(int timesteps, int simulations,
                                                                                double (*stat_func_call)(const std::vector<double> &),
                                                                                double (*stat_func_put)(const std::vector<double> &),
                                                                                std::function<double(const double)> pay_off_func_call,
                                                                                std::function<double(const double)> pay_off_func_put) const;

  protected:
    std::unique_ptr<PayOffFixedStrike> pay_off;
};

class FloatingStrikeOption : public Option {
  public:
    FloatingStrikeOption(std::unique_ptr<PayOffFloatingStrike> pay_off, std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params);
    virtual ~FloatingStrikeOption();

    virtual std::tuple<double, double, ErrorData, ErrorData>
    price_floating_strike(int timesteps, int simulations, double (*stat_func_call)(const std::vector<double> &),
                          double (*stat_func_put)(const std::vector<double> &),
                          std::function<double(const double, const double)> call_pay_off_call,
                          std::function<double(const double, const double)> pay_off_func_put) const;

  protected:
    std::unique_ptr<PayOffFloatingStrike> pay_off;
};

class AsianFixedStrikeOption : public FixedStrikeOption {
  public:
    AsianFixedStrikeOption(std::unique_ptr<PayOffFixedStrike> pay_off, std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params);
    ~AsianFixedStrikeOption();

    std::tuple<double, double, ErrorData, ErrorData> operator()(int timesteps, int simulations) const override;
};

class AsianFloatingStrikeOption : public FloatingStrikeOption {
  public:
    AsianFloatingStrikeOption(std::unique_ptr<PayOffFloatingStrike> pay_off, std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params);
    ~AsianFloatingStrikeOption();

    std::tuple<double, double, ErrorData, ErrorData> operator()(int timesteps, int simulations) const override;
};

class LookbackFixedStrikeOption : public FixedStrikeOption {
  public:
    LookbackFixedStrikeOption(std::unique_ptr<PayOffFixedStrike> pay_off, std::shared_ptr<MonteCarlo> mc, OptionsParams &options_params);
    ~LookbackFixedStrikeOption();

    std::tuple<double, double, ErrorData, ErrorData> operator()(int timesteps, int simulations) const override;
};

class LookbackFloatingStrikeOption : public FloatingStrikeOption {
  public:
    LookbackFloatingStrikeOption(std::unique_ptr<PayOffFloatingStrike> pay_off, std::shared_ptr<MonteCarlo> mc,
                                 OptionsParams &options_params);
    ~LookbackFloatingStrikeOption();

    std::tuple<double, double, ErrorData, ErrorData> operator()(int timesteps, int simulations) const override;
};

#endif