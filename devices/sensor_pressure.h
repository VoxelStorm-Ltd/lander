#ifndef SENSOR_PRESSURE_H_INCLUDED
#define SENSOR_PRESSURE_H_INCLUDED

#include "device.h"

class sensor_pressure : public device {
  /// Static pressure sensor
  // stats vaguely based on http://www.abb.com/product/seitp330/a1d23674d8a8cc85c125762c00500889.aspx
private:
  double pressure;                                    // last pressure in Pa
  double static constexpr pressurelimit = 15000000;   // max survivable pressure in Pa ~= 15MPa (surface of venus = 9.2Mpa)
  double static constexpr temperaturelimit = 800;     // max survivable temperature in Pa (venus ~= 740K)

public:
  sensor_pressure();
  ~sensor_pressure();

  std::string  get_name();
  std::string  get_description();
  unsigned int get_port_out_count();
  std::string  get_port_out_name(       unsigned int port);
  std::string  get_port_out_description(unsigned int port);
  double       get_port_out_data(       unsigned int port);
  void update();
};

#endif // SENSOR_PRESSURE_H_INCLUDED
