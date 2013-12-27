#include "operator_sub.h"

operator_sub::operator_sub() {
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this
}

operator_sub::~operator_sub() {
  /// Default destructor
}

std::string operator_sub::get_name() {
  /// Return a name for this device
  return "subtracter";
}

std::string operator_sub::get_model() {
  /// Return a model name for this device
  return "OP-SUB";
}

std::string operator_sub::get_description() {
  /// Return a detailed description of this device
  return "A subtraction operator device.  Returns the result of input 2 subtracted from input 1.";
}

std::string operator_sub::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  std::stringstream desc;
  if(port == 0) {
    desc << "Value to subtract input 2 from (LHS)";
  } else {
    desc << "Value to subtract from input 1 (RHS)";
  }
  desc << "  " << get_port_in_connstatus(port);
  return desc.str();
}


std::string operator_sub::get_port_out_description(unsigned int port  __attribute__((__unused__))) {
  /// Description of the output port numbered n
  return "A numerical data value representing the result of subtracting input 2 from input 1.";
}

double operator_sub::get_port_out_data(unsigned int port __attribute__((__unused__))) {
  /// Query the value data on the specified out port
  if(ports_in[0].target && ports_in[1].target) {
    return ports_in[0].target->get_port_out_data(ports_in[0].target_port) -
           ports_in[1].target->get_port_out_data(ports_in[1].target_port);
  } else {
    return 0.0;
  }
}
