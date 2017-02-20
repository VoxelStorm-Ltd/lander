#include "sensor_pressure.h"
#include <sstream>
#include "vectorstorm/vectorstorm.h"
#include "universe.h"
#include "starsystem.h"
#include "orbitalbody.h"
#include "spacecraft.h"

extern universe root;

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

std::string sensor_pressure::get_model() {
  /// Return a model name for this device
  std::stringstream model;
  model << "PS-" << static_cast<int>(pressurelimit / 1000000) << "-" << static_cast<int>(temperaturelimit / 100);
  return model.str();
}

std::string sensor_pressure::get_description() {
  /// Return a detailed description of this device
  random_reset();
  std::stringstream desc;
  desc << "A pressure sensor that monitors external ambient static pressure.  Capable of withstanding up to "
       << pressurelimit << "Pa pressure and "
       << temperaturelimit << "K temperature.";
  return desc.str();
}

double sensor_pressure::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.35;
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

  // iterate through every sufficiently significant body
  orbitalbody *thisbody = nullptr;
  double distance;
  for(auto *it : root.currentsystem->bodies) {
    // only check planets and stars etc
    thisbody = dynamic_cast<orbitalbody*>(it);
    if(!thisbody) {
      continue;
    }
    distance = Vector3d(thisbody->position - vessel->position).length();
    // are we within its sphere of influence?
    if(thisbody->check_within_physical_influence(distance)) {
      // break out of the loop on first hit, since we're unlikely to be in two atmospheres at once
      break;
    }
  }
  if(!thisbody) {
    // we're not in any planet's sphere of influence
    pressure = 0.0;
    return;
  }
  pressure                 = thisbody->get_atmos_pressure(   distance);
  double const temperature = thisbody->get_atmos_temperature(distance) + vessel->get_temperature_hull();

  if(pressure > pressurelimit) {
    std::cout << "INFO: static pressure sensor destroyed by overpressure (" << pressure << "Pa, limit " << pressurelimit << "Pa)" << std::endl;
    destroy();
  }
  if(temperature > temperaturelimit) {
    std::cout << "INFO: static pressure sensor destroyed by overheating (" << temperature << "K, limit " << temperaturelimit << "K)" << std::endl;
    destroy();
  }
}
