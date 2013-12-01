#include "sensor_pressure.h"
#include <sstream>

sensor_pressure::sensor_pressure()
  : pressure(0.0) {
  /// Default constructor
}

sensor_pressure::~sensor_pressure() {
  /// Default destructor
}

std::string sensor_pressure::get_name() {
  /// Return the name of this device
  return "static pressure sensor";
}

std::string sensor_pressure::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << get_random_name_corporation() << " model PS-" << static_cast<int>(pressurelimit / 1000000) << "-" << static_cast<int>(temperaturelimit / 100)
       << ".  A pressure sensor that monitors external ambient static pressure.  Capable of withstanding up to "
       << pressurelimit << "Pa pressure and "
       << temperaturelimit << "K temperature.";
  return desc.str();
}

unsigned int sensor_pressure::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string sensor_pressure::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "pressure";
}

std::string sensor_pressure::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "External static pressure in Pa.";
}

double sensor_pressure::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the data on the specified out port
  if(functional) {
    update();
  }
  return pressure;
}

void sensor_pressure::update() {
  /// Update the output states and respond to changes in input
  // work out if we're within the atmosphere of a planet
  // get the pressure at our depth

  // TODO
  //pressure = thisplanet->get_pressure(position)
  double const temperature = 200.0;

  if(pressure > pressurelimit) {
    destroy();
  }
  if(temperature > temperaturelimit) {
    destroy();
  }
}
