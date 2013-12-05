#ifndef ALTIMETER_H_INCLUDED
#define ALTIMETER_H_INCLUDED

#include "instrument.h"

class altimeter : public instrument {
private:
  double altitude;

public:
  altimeter();
  ~altimeter();

  std::string get_name();
  std::string  get_model();
  std::string get_description();
  unsigned int get_port_in_count();
  std::string get_port_in_name(       unsigned int port);
  std::string get_port_in_description(unsigned int port);
  bool get_port_in_required(          unsigned int port);
  void update();
  void render();
};

#endif // ALTIMETER_H_INCLUDED
