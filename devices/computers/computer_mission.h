#ifndef COMPUTER_MISSION_H_INCLUDED
#define COMPUTER_MISSION_H_INCLUDED

#include <queue>
#include "device.h"

class computer_mission : public device {
  /// Mission control computer
private:
  std::queue<std::string> buffer;                                               // output line buffer, no size limit

public:
  computer_mission();
  ~computer_mission();

  std::string  get_name() override final;
  std::string  get_model() override final;
  std::string  get_description() override final;
  vector3d     get_size() override final;
  double       get_mass() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(        unsigned int port) override final;
  std::string  get_port_in_description( unsigned int port) override final;
  bool         get_port_in_required(    unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(       unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  std::string  get_port_out_text(       unsigned int port) override final;

  // computer-specific functions
  void boot();
};

#endif // COMPUTER_MISSION_H_INCLUDED
