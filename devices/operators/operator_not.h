#ifndef OPERATOR_NOT_H_INCLUDED
#define OPERATOR_NOT_H_INCLUDED

#include "operator_base.h"

class operator_not : public operator_base {
  /// NOT operator device.  Takes one inputs and outputs 1 iff it's zero
public:
  operator_not();
  ~operator_not();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_description( unsigned int port) override final;
  bool         get_port_in_required(    unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  double       get_port_out_data(       unsigned int port) override final;
};

#endif // OPERATOR_NOT_H_INCLUDED
