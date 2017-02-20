#ifndef OPERATOR_OR_H_INCLUDED
#define OPERATOR_OR_H_INCLUDED

#include "operator_base.h"

class operator_or : public operator_base {
  /// OR operator device.  Takes two inputs and outputs 1 if either is non-zero
public:
  operator_or();
  ~operator_or();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  std::string  get_port_in_description( unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  double       get_port_out_data(       unsigned int port) override final;
};

#endif // OPERATOR_OR_H_INCLUDED
