#include "operator_base.h"

operator_base::operator_base() {
  /// Default constructor
}

operator_base::~operator_base() {
  /// Default destructor
}

std::string operator_base::get_manufacturer() {
  /// Return a manufacturer string for this device
  return "Multivac";
}

double operator_base::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.05;
}

Vector3d operator_base::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.03, 0.01, 0.01);
}

unsigned int operator_base::get_port_in_count() {
  /// How many input ports it has
  // most will default to 2
  return 2;
}

std::string operator_base::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  return "input";
}

bool operator_base::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are mandatory
  return true;
}

unsigned int operator_base::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string operator_base::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "output";
}
