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
  std::string get_name();
  std::string get_model();
  std::string get_description();
  double      get_mass();
  Vector3d    get_size();
  bool attach_panel(instrumentpanel *to_panel);
  bool attach_hull();
  bool attach_cabin();
  void remove_panel();
  void remove_hull();
  void remove_cabin();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(        unsigned int port);
  std::string  get_port_in_description( unsigned int port);
  bool         get_port_in_required(    unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(       unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  double       get_port_out_data(       unsigned int port);
};

#endif // ATTITUDE_CMG_H_INCLUDED
