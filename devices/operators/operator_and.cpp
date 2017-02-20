#include "operator_and.h"

operator_and::operator_and() {
  /// Default constructor
  ports_in.resize(get_port_in_count());                                         // anything with input ports needs this
}

operator_and::~operator_and() {
  /// Default destructor
}

std::string operator_and::get_name() {
  /// Return a name for this device
  return "AND gate";
}

std::string operator_and::get_model() {
  /// Return a model name for this device
  return "OP-AND";
}

std::string operator_and::get_description() {
  /// Return a detailed description of this device
  return "An AND operator device.  Outputs 1 if both inputs are non-zero.";
}

std::string operator_and::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "Input value for AND operation."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}

std::string operator_and::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical value equal to 1 if both inputs are non-zero, and 0 otherwise.";
}

double operator_and::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[0].target && ports_in[1].target) {
    if(ports_in[0].target->get_port_out_data(ports_in[0].target_port) != 0.0 &&
       ports_in[1].target->get_port_out_data(ports_in[1].target_port) != 0.0) {
      return 1.0;
    } else {
      return 0.0;
    }
  } else {
    return 0.0;
  }
}
