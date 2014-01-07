#ifndef OPERATOR_DIV_INCLUDED
#define OPERATOR_DIV_INCLUDED

#include "operator_base.h"

class operator_div : public operator_base {
  /// divtiplication operator device.  Takes two inputs and outputs input 1 divided by input 2.
public:
  operator_div();
  ~operator_div();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  std::string  get_port_in_description( unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  double       get_port_out_data(       unsigned int port);
};

#endif // OPERATOR_DIV_INCLUDED
