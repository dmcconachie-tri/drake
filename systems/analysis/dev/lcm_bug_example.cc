#include <iostream>

#include "drake/lcmt_scope.hpp"
#include "drake/lcm/drake_lcm.h"
#include "drake/systems/analysis/simulator.h"
#include "drake/systems/framework/diagram.h"
#include "drake/systems/framework/diagram_builder.h"
#include "drake/systems/lcm/lcm_interface_system.h"
#include "drake/systems/lcm/lcm_scope_system.h"
#include "drake/systems/primitives/sine.h"

namespace drake {
namespace systems {
namespace {

void NoProblem() {
  std::cout << "\n\n\n"
               "-------- -------- NoProblem -------- --------\n";

  const std::string channel = "CHANNEL";
  const double publish_period = 0.05;
  DiagramBuilder<double> builder;
  auto lcm = builder.AddSystem<lcm::LcmInterfaceSystem>();
  auto source = builder.AddSystem<Sine>(
      Eigen::Vector2d(1.0,        2.0),          // amplitudes
      Eigen::Vector2d(2.0 * M_PI, 2 * M_PI),     // frequencies = 1 rev / sec
      Eigen::Vector2d(0.0,        0.5 * M_PI));  // phases
  auto [scope, publisher] = lcm::LcmScopeSystem::AddToBuilder(
      &builder, lcm, source->get_output_port(0), channel, publish_period);
  (void)scope;
  (void)publisher;

  // Simulate and listen for received messages.
  Simulator<double> simulator(builder.Build());
  simulator.AdvanceTo(publish_period * 5);
}

void ShowProblem() {
  std::cout << "\n\n\n"
               "-------- -------- ShowProblem -------- --------\n";

  const std::string channel = "CHANNEL";
  const double publish_period = 0.05;
  DiagramBuilder<double> builder;
  auto lcm = builder.AddSystem<lcm::LcmInterfaceSystem>();
  auto source = builder.AddSystem<Sine>(
      Eigen::Vector2d(1.0,        2.0),          // amplitudes
      Eigen::Vector2d(2.0 * M_PI, 2 * M_PI),     // frequencies = 1 rev / sec
      Eigen::Vector2d(0.0,        0.5 * M_PI));  // phases
  auto [scope, publisher] = lcm::LcmScopeSystem::AddToBuilder(
      &builder, lcm, source->get_output_port(0), channel, publish_period);
  (void)scope;
  (void)publisher;

  // Simulate and listen for received messages.
  drake::lcm::Subscriber<lcmt_scope> subscriber(lcm, channel);
  Simulator<double> simulator(builder.Build());
  simulator.AdvanceTo(publish_period * 5);
}



}
}
}

int main(int, char **) {
  drake::systems::NoProblem();
  drake::systems::ShowProblem();
  std::cout << "\n\n\n"
               "-------- -------- -------- --------\n"
               "Compare how many items are published in each example\n";
  return EXIT_SUCCESS;
}
