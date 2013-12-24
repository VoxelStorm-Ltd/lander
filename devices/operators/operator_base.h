#ifndef OPERATOR_BASE_H_INCLUDED
#define OPERATOR_BASE_H_INCLUDED

#include "device.h"

class operator_base : public device {
  /// Convenience base class for all operators to define those things they all have in common
protected:                              // prevent this class being instantiated directly
  operator_base();
public:
  ~operator_base();

  std::string  get_manufacturer();
  Vector3d     get_size();
  double       get_mass();
  std::string  get_port_in_name(    unsigned int port);
  bool         get_port_in_required(unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(   unsigned int port);
};

#endif // OPERATOR_BASE_H_INCLUDED
