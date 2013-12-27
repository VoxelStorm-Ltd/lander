#ifndef OPERATOR_SUB_H_INCLUDED
#define OPERATOR_SUB_H_INCLUDED

#include "operator_base.h"

class operator_sub : public operator_base {
  /// subition operator device.  Takes two inputs and outputs the sum result.
public:
  operator_sub();
  ~operator_sub();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  std::string  get_port_in_description( unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  double       get_port_out_data(       unsigned int port);
};

#endif // OPERATOR_SUB_H_INCLUDED
