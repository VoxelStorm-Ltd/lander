#ifndef OPERATOR_MUL_H_INCLUDED
#define OPERATOR_MUL_H_INCLUDED

#include "operator_base.h"

class operator_mul : public operator_base {
  /// Multiplication operator device.  Takes two inputs and outputs the multiplied result.
public:
  operator_mul();
  ~operator_mul();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  unsigned int get_port_in_count();
  std::string  get_port_in_description( unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  double       get_port_out_data(       unsigned int port);
};

#endif // OPERATOR_MUL_H_INCLUDED
