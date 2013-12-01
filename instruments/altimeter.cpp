#include "altimeter.h"

altimeter::altimeter()
  : altitude(0.0) {
  /// Default constructor
  port_in_type const port1 = {
    nullptr,
    0
  };
  ports_in.push_back(port1);
}

altimeter::~altimeter() {
  /// Default destructor
}


std::string altimeter::get_name() {
  /// Return the name of this device
  // virtual placeholder
  return "barometric altimeter";
}

std::string altimeter::get_description() {
  /// Return a detailed description of this device
  // virtual placeholder
  return "A simple altimeter that calculates altitude based on static atmospheric pressure."
         "  Calibrated for use in Earth's atmosphere only."
         "  Not capable of giving meaningful results outside of an atmosphere.";
}

unsigned int altimeter::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string altimeter::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "static pressure";
  default:
    return "";
  }
}

std::string altimeter::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "A numerical reading of the static pressure outside the vessel, in Pa (N/m2).";
      if(ports_in[port].target) {
        desc << "  Connected to \"" << ports_in[port].target->get_port_out_name(ports_in[port].target_port)
             << "\" port of " << ports_in[port].target->get_name() << ".";
      } else {
        desc << "  Not connected.";
      }
      if(get_port_in_required(port)) {
        desc << "  Connection required for correct operation.";
      }
      return desc.str();
    }
  default:
    return "";
  }
}

bool altimeter::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are needed
  return true;
}

void altimeter::update() {
  /// Update the readings on this instrument and re-cache new values
  if(!ports_in[0].target) {
    // not connected
    altitude = 0.0;
    return;
  }
  double pressure = ports_in[0].target->query_port(ports_in[0].target_port);

  if(pressure == 0.0) {
    altitude = 0.0;
    return;
  }
  // pressure = 101325 * ((1 - (altitude * 2.25577 * 10^-5))^5.25588)
  // pressure_sealevel = 101325 Pa
  // altitude = ((10 * log10(pressure / pressure_sealevel) / 5.2558797) - 1) / (-6.8755856 * 10^-6)
  altitude = (((10.0 / 5.2558797) * log10(pressure / 101325)) - 1.0) / -0.0000068755856;
}
