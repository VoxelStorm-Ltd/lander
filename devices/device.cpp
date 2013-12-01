#include "device.h"

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

std::string device::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  // virtual placeholder
  return "";
}

std::string device::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  // virtual placeholder
  return "";
}

bool device::get_port_in_required(unsigned int port) {
  /// Whether an input on this port is necessary for this device to operate
  // virtual placeholder
  return false;
}

unsigned int device::get_port_out_count() {
  /// How many output ports it has
  // virtual placeholder
  return 0;
}

std::string device::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  // virtual placeholder
  return "";
}

std::string device::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  // virtual placeholder
  return "";
}

double device::query_port(unsigned int port) {
  /// Query the data on the specified out port
  // virtual placeholder
  return 0.0;
}

void device::update() {
  /// Update the output states and respond to changes in input
  // virtual placeholder
}
