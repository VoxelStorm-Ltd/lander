#ifndef OPERATOR_ADD_H_INCLUDED
#define OPERATOR_ADD_H_INCLUDED

#include "operator_base.h"

class operator_add : public operator_base {
  /// Addition operator device.  Takes two inputs and outputs the sum result.
public:
  operator_add();
  ~operator_add();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  std::string  get_port_in_description( unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  double       get_port_out_data(       unsigned int port);
};

#endif // OPERATOR_ADD_H_INCLUDED
