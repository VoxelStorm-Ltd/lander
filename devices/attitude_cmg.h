#ifndef ATTITUDE_CMG_H_INCLUDED
#define ATTITUDE_CMG_H_INCLUDED

#include "device.h"

class attitude_cmg : public device {
  /// Motion control system - control moment gyro
  /// see http://www.boeing.com/assets/pdf/defense-space/space/spacestation/systems/docs/ISS%20Motion%20Control%20System.pdf
public:
  attitude_cmg();
  ~attitude_cmg();

  // specialisations
  std::string get_name() override final;
  std::string get_model() override final;
  std::string get_description() override final;
  double      get_mass() override final;
  Vector3d    get_size() override final;
  bool attach_panel(instrumentpanel *to_panel) override final;
  bool attach_hull() override final;
  bool attach_cabin() override final;
  void remove_panel() override final;
  void remove_hull() override final;
  void remove_cabin() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(        unsigned int port) override final;
  std::string  get_port_in_description( unsigned int port) override final;
  bool         get_port_in_required(    unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(       unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  double       get_port_out_data(       unsigned int port) override final;
};

#endif // ATTITUDE_CMG_H_INCLUDED
