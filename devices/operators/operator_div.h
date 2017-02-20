#ifndef OPERATOR_DIV_INCLUDED
#define OPERATOR_DIV_INCLUDED

#include "operator_base.h"

class operator_div : public operator_base {
  /// divtiplication operator device.  Takes two inputs and outputs input 1 divided by input 2.
public:
  operator_div();
  ~operator_div();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  std::string  get_port_in_description( unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  double       get_port_out_data(       unsigned int port) override final;
};

#endif // OPERATOR_DIV_INCLUDED
