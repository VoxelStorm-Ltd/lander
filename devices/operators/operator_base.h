#ifndef OPERATOR_BASE_H_INCLUDED
#define OPERATOR_BASE_H_INCLUDED

#include "device.h"

class operator_base : public device {
  /// Convenience base class for all operators to define those things they all have in common
protected:                                                                      // prevent this class being instantiated directly
  operator_base();
public:
  virtual ~operator_base();

  virtual std::string  get_manufacturer() override final;
  virtual Vector3d     get_size() override final;
  virtual double       get_mass() override final;
  virtual unsigned int get_port_in_count() override;
  virtual std::string  get_port_in_name(    unsigned int port) override final;
  virtual bool         get_port_in_required(unsigned int port) override;
  virtual unsigned int get_port_out_count() override final;
  virtual std::string  get_port_out_name(   unsigned int port) override final;
};

#endif // OPERATOR_BASE_H_INCLUDED
