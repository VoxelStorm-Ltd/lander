#include "operator_not.h"

operator_not::operator_not() {
  /// Default constructnot
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
}

operator_not::~operator_not() {
  /// Default destructnot
}

std::string operator_not::get_name() {
  /// Return a name for this device
  return "NOT gate";
}

std::string operator_not::get_model() {
  /// Return a model name for this device
  return "OP-NOT";
}

std::string operator_not::get_description() {
  /// Return a detailed description of this device
  return "An NOT operator device.  Outputs 0 if the input is non-zero, and 1 otherwise.";
}

unsigned int operator_not::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string operator_not::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "Input value for NOT operation."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

bool operator_not::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // input is optional - it'll return 1 if nothing is connected
  return false;
}

std::string operator_not::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical value equal to 0 if and only if the input input is non-zero, and 1 otherwise.";
}

double operator_not::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[0].target) {
    if(ports_in[0].target->get_port_out_data(ports_in[0].target_port) == 0.0) {
      return 1.0;
    } else {
      return 0.0;
    }
  } else {
    return 1.0;
  }
}
