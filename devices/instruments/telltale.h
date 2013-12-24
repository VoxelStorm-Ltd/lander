#ifndef TELLTALE_H_INCLUDED
#define TELLTALE_H_INCLUDED

#include "instrument.h"

class telltale : public instrument {
  /// Basic on/off indicator light displaying a fixed text label
public:
  telltale();
  ~telltale();

  std::string  get_name();
  std::string  get_model();
  std::string  get_description();
  Vector3d     get_size();
  double       get_mass();
  unsigned int get_port_in_count();
  std::string  get_port_in_name(       unsigned int port);
  std::string  get_port_in_description(unsigned int port);
  bool         get_port_in_required(   unsigned int port);

  void render();
};

#endif // TELLTALE_H_INCLUDED
