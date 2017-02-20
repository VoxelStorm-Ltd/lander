#ifndef OPERATOR_AND_INCLUDED
#define OPERATOR_AND_INCLUDED

#include "operator_base.h"

class operator_and : public operator_base {
  /// AND operator device.  Takes two inputs and outputs 1 if both are non-zero
public:
  operator_and();
  ~operator_and();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  std::string  get_port_in_description( unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  double       get_port_out_data(       unsigned int port) override final;
};

#endif // OPERATOR_AND_INCLUDED
