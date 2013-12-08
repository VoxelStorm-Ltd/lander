#include "display_digital.h"

display_digital::display_digital() {
  /// Default constructor
}

display_digital::~display_digital() {
  /// Default destructor
}

std::string display_digital::get_name() {
  /// Return the name of this device
  return "digital graphical display";
}

std::string display_digital::get_model() {
  /// Return a model name for this device
  random_reset();
  unsigned int const modelnum = get_random_int(1, 9);
  std::stringstream model;
  model << "LCD" << modelnum << modelnum << "00-D";
  return model.str();
}

std::string display_digital::get_description() {
  /// Return a detailed description of this device
  return "A digital LCD monitor, much flatter and lighter than its CRT equivalents "
         "but considerably more susceptible to physical and electrical damage."
         "  Purists complain about the slow response time and the fact that black "
         "is never really completely black on these things."
         "  Useful for displaying digital signals from older devices that don't "
         "offer analogue output.";
}

double display_digital::get_mass() {
  /// Return the weight of the device, in kilograms
  return 2.5;
}

void display_digital::update() {
  /// Update the contents of this display - digital version
  if(ports_in[0].target) {
    display_image = ports_in[0].target->get_port_out_video_digital(ports_in[0].target_port);
  } else {
    display_image = generate_static_digital();
    // TODO: overlay "no signal" text on static
  }
}
