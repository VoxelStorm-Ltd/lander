#ifndef COMPUTER_MISSION_H_INCLUDED
#define COMPUTER_MISSION_H_INCLUDED

#include <queue>
#include "device.h"

class computer_mission : public device {
  /// Mission control computer
private:
  std::queue<std::string> buffer;       // output line buffer, no size limit

public:
  computer_mission();
  ~computer_mission();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  Vector3d     get_size();
  double       get_mass();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(        unsigned int port);
  std::string  get_port_in_description( unsigned int port);
  bool         get_port_in_required(    unsigned int port);
  unsigned int get_port_out_count();
  std::string  get_port_out_name(       unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  std::string  get_port_out_text(       unsigned int port);

  // computer-specific functions
  void boot();
};

#endif // COMPUTER_MISSION_H_INCLUDED
