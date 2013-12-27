#include "operator_add.h"

operator_add::operator_add() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

operator_add::~operator_add() {
  /// Default destructor
}

std::string operator_add::get_name() {
  /// Return a name for this device
  return "adder";
}

std::string operator_add::get_model() {
  /// Return a model name for this device
  return "OP-ADD";
}

std::string operator_add::get_description() {
  /// Return a detailed description of this device
  return "An addition operator device.  Takes two inputs and outputs the sum.";
}

std::string operator_add::get_port_in_description(unsigned int port __attribute__((__unused__))) {
  /// Description of the input port numbered n
  std::stringstream desc;
  desc << "Input value to add."
          "  " << get_port_in_connstatus(port);
  return desc.str();
}


std::string operator_add::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical data value representing the result of adding together the two inputs.";
}

double operator_add::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[0].target && ports_in[1].target) {
    return ports_in[0].target->get_port_out_data(ports_in[0].target_port) +
           ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  } else {
    return 0.0;
  }
}
