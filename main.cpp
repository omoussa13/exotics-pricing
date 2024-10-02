#include "MonteCarlo.hpp"
#include "Option.hpp"
#include "Simulation.hpp"
#include <iostream>
#include <memory>

const int SIMULATIONS = 10000;
const int TIMESTEPS = 252;
const OptionsParams INITIAL_OTION_PARAMETERS = {100.0, 100.0, 1.0, 0.2, 0.05};

void run_sim(Simulation *simulator, void (Simulation::*func)(), bool is_fixed_strike) {
  (simulator->*func)();
  std::cout << *simulator << std::endl;

  // simulator->change_S0(90.0);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;

  // simulator->change_S0(95.0);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;

  // simulator->change_S0(110.0);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;

  // // strike price only influences fixed strike options
  // if (is_fixed_strike) {
  //   simulator->reset_params(INITIAL_OTION_PARAMETERS);
  //   simulator->change_E(90.0);
  //   (simulator->*func)();
  //   std::cout << *simulator << std::endl;

  //   simulator->change_E(95.0);
  //   (simulator->*func)();
  //   std::cout << *simulator << std::endl;

  //   simulator->change_E(110.0);
  //   (simulator->*func)();
  //   std::cout << *simulator << std::endl;
  // }

  simulator->reset_params(INITIAL_OTION_PARAMETERS);
  simulator->change_T(0.5);
  (simulator->*func)();
  std::cout << *simulator << std::endl;

  simulator->reset_params(INITIAL_OTION_PARAMETERS);
  simulator->change_T(0.75);
  (simulator->*func)();
  std::cout << *simulator << std::endl;

  simulator->reset_params(INITIAL_OTION_PARAMETERS);
  simulator->change_T(1.5);
  (simulator->*func)();
  std::cout << *simulator << std::endl;

  // simulator->reset_params(INITIAL_OTION_PARAMETERS);
  // simulator->change_sigma(0.1);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;

  // simulator->reset_params(INITIAL_OTION_PARAMETERS);
  // simulator->change_r(0.01);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;

  // simulator->change_r(0.03);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;

  // simulator->change_r(0.07);
  // (simulator->*func)();
  // std::cout << *simulator << std::endl;
}

void run_asian_fixed_strike_sim(std::shared_ptr<MonteCarlo> mc) {
  Simulation *s = new Simulation("Asian Option Fixed Strike", TIMESTEPS, SIMULATIONS, mc, INITIAL_OTION_PARAMETERS);
  run_sim(s, &Simulation::simulate_asian_option_fixed_strike, true);

  delete s;
  s = nullptr;
}

void run_asian_floating_strike_sim(std::shared_ptr<MonteCarlo> mc) {
  Simulation *s = new Simulation("Asian Option floating Strike", TIMESTEPS, SIMULATIONS, mc, INITIAL_OTION_PARAMETERS);
  run_sim(s, &Simulation::simulate_asian_option_floating_strike, false);

  delete s;
  s = nullptr;
}

void run_lookback_fixed_strike(std::shared_ptr<MonteCarlo> mc) {
  Simulation *s = new Simulation("Lookback Option Fixed Strike", TIMESTEPS, SIMULATIONS, mc, INITIAL_OTION_PARAMETERS);
  run_sim(s, &Simulation::simulate_lookback_option_fixed_strike, true);

  delete s;
  s = nullptr;
}

void run_lookback_floating_strike_sim(std::shared_ptr<MonteCarlo> mc) {
  Simulation *s = new Simulation("Lookback Option floating Strike", TIMESTEPS, SIMULATIONS, mc, INITIAL_OTION_PARAMETERS);
  run_sim(s, &Simulation::simulate_lookback_option_floating_strike, false);

  delete s;
  s = nullptr;
}

int main() {
  std::shared_ptr<MonteCarlo> mc = std::make_shared<MonteCarlo>();

  run_asian_fixed_strike_sim(mc);
  run_asian_floating_strike_sim(mc);
  run_lookback_fixed_strike(mc);
  run_lookback_floating_strike_sim(mc);

  return 0;
}