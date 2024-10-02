#include "PayOff.hpp"
#include <algorithm>

PayOff::PayOff() {}

PayOff::~PayOff() {}

PayOffFixedStrike::PayOffFixedStrike(double strike) : strike(strike) {}

PayOffFixedStrike::~PayOffFixedStrike() {}

PayOffFloatingStrike::PayOffFloatingStrike() {}

PayOffFloatingStrike::~PayOffFloatingStrike() {}

// Asian payoffs

AsianFixedStrikePayOff::AsianFixedStrikePayOff(double strike) : PayOffFixedStrike(strike) {}

AsianFixedStrikePayOff::~AsianFixedStrikePayOff() {}

double AsianFixedStrikePayOff::call(const double avg) const {
  return std::max(avg - this->strike, 0.0);
}

double AsianFixedStrikePayOff::put(const double avg) const {
  return std::max(this->strike - avg, 0.0);
}

AsianFloatingStrikePayOff::AsianFloatingStrikePayOff() {}

AsianFloatingStrikePayOff::~AsianFloatingStrikePayOff() {}

double AsianFloatingStrikePayOff::call(const double avg, const double s) const {
  return std::max(s - avg, 0.0);
}

double AsianFloatingStrikePayOff::put(const double avg, const double s) const {
  return std::max(avg - s, 0.0);
}

// Lookback payoffs

LookbackFixedStrikePayOff::LookbackFixedStrikePayOff(double strike) : PayOffFixedStrike(strike) {}

LookbackFixedStrikePayOff::~LookbackFixedStrikePayOff() {}

double LookbackFixedStrikePayOff::call(const double max) const {
  return std::max(max - this->strike, 0.0);
}

double LookbackFixedStrikePayOff::put(const double min) const {
  return std::max(this->strike - min, 0.0);
}

LookbackFloatingStrikePayOff::LookbackFloatingStrikePayOff() {}

LookbackFloatingStrikePayOff::~LookbackFloatingStrikePayOff() {}

double LookbackFloatingStrikePayOff::call(const double min, const double s) const {
  return std::max(s - min, 0.0);
}

double LookbackFloatingStrikePayOff::put(const double max, const double s) const {
  return std::max(max - s, 0.0);
}