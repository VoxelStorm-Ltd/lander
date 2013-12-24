#include "operator_mul.h"

operator_mul::operator_mul() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

operator_mul::~operator_mul() {
  /// Default destructor
}

std::string operator_mul::get_name() {
  /// Return a name for this device
  return "multiplier";
}

std::string operator_mul::get_manufacturer() {
  /// Return a manufacturer string for this device
  return "Multivac";
}

std::string operator_mul::get_model() {
  /// Return a model name for this device
  return "OP-MUL";
}

std::string operator_mul::get_description() {
  /// Return a detailed description of this device
  return "A multiplication operator device.  Takes two inputs and outputs the multiplied result.";
}

double operator_mul::get_mass() {
  /// Return the weight of the device, in kilograms
  return 0.05;
}

Vector3d operator_mul::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(0.03, 0.01, 0.01);
}

unsigned int operator_mul::get_port_in_count() {
  /// How many input ports it has
  return 2;
}

std::string operator_mul::get_port_in_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the input port numbered n
  return "input";
}

std::string operator_mul::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "Input value to multiply."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

bool operator_mul::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all inputs are mandatory
  return true;
}

unsigned int operator_mul::get_port_out_count() {
  /// How many output ports it has
  return 1;
}

std::string operator_mul::get_port_out_name(unsigned int port __attribute__((__unused__))) {
  /// Name of the output port numbered n
  return "output";
}

std::string operator_mul::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical data value representing the result of multiplying together the two inputs.";
}

double operator_mul::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[0].target && ports_in[1].target) {
    return ports_in[0].target->get_port_out_data(ports_in[0].target_port) *
           ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  } else {
    return 0.0;
  }
}
