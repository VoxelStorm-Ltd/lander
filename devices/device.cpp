#include "device.h"
#include <sstream>

device::device() {
  /// Default constructor
}

device::~device() {
  /// Default destructor
}

std::string device::get_name() {
  /// Return the name of this device
  // virtual placeholder
  return "Device";
}

std::string device::get_description() {
  /// Return a detailed description of this device
  // virtual placeholder
  return "A device of some sort.  It doesn't appear to do anything.";
}

unsigned int device::get_port_in_count() {
  /// How many input ports it has
  // virtual placeholder
  return 0;
}

std::string device::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  // virtual placeholder
  return "";
}

std::string device::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  // virtual placeholder
  return "";
}

bool device::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // virtual placeholder
  return false;
}

std::string device::get_port_in_connstatus(unsigned int port) {
  /// Get a verbal description of the connection status of this input port
  std::stringstream desc;
  if(ports_in[port].target) {
    desc << "Connected to \"" << ports_in[port].target->get_port_out_name(ports_in[port].target_port)
         << "\" port of " << ports_in[port].target->get_name() << ".";
  } else {
    desc << "Not connected";
    if(get_port_in_required(port)) {
      desc << " (required).";
    } else {
      desc << " (optional).";
    }
  }
  return desc.str();
}

unsigned int device::get_port_out_count() {
  /// How many output ports it has
  // virtual placeholder
  return 0;
}

std::string device::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  // virtual placeholder
  return "";
}

std::string device::get_port_out_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the output port numbered n
  // virtual placeholder
  return "";
}

double device::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the data on the specified out port
  // virtual placeholder
  return 0.0;
}

void device::update() {
  /// Update the output states and respond to changes in input
  // virtual placeholder
}
