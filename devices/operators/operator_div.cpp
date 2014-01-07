#include "operator_div.h"

operator_div::operator_div() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

operator_div::~operator_div() {
  /// Default destructor
}

std::string operator_div::get_name() {
  /// Return a name for this device
  return "divider";
}

std::string operator_div::get_model() {
  /// Return a model name for this device
  return "OP-DIV";
}

std::string operator_div::get_description() {
  /// Return a detailed description of this device
  return "A division operator device.  Takes two inputs and outputs input 1 divided by input 2.";
}

std::string operator_div::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  std::stringstream desc;
  if(port == 0) {
    desc << "Input value to divide.";
  } else {
    desc << "Input value to divide by.";
  }
  desc << "  " << get_port_in_connstatus(port);
  return desc.str();
}

std::string operator_div::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical data value representing the result of dividing input 1 by input 2.";
}

double operator_div::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[0].target && ports_in[1].target) {
    return ports_in[0].target->get_port_out_data(ports_in[0].target_port) /
           ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  } else {
    return 0.0;
  }
}
