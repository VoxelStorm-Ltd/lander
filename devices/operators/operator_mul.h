#ifndef OPERATOR_MUL_H_INCLUDED
#define OPERATOR_MUL_H_INCLUDED

#include "device.h"

class operator_mul : public device {
  /// Multiplication operator device.  Takes two inputs and outputs the multiplied result.
public:
  operator_mul();
  ~operator_mul();

  std::string  get_name();
  std::string  get_manufacturer();
  std::string  get_model();
  std::string  get_description();
  Vector3d     get_size();
  double       get_mass();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(           unsigned int port);
  std::string  get_port_in_description(    unsigned int port);
  bool         get_port_in_required(       unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(          unsigned int port);
  std::string  get_port_out_description(   unsigned int port);
  double       get_port_out_data(          unsigned int port);
};

#endif // OPERATOR_MUL_H_INCLUDED
