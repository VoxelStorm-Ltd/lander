#ifndef THRUSTER_H_INCLUDED
#define THRUSTER_H_INCLUDED

#include "device.h"

class thruster : public device {
private:
  double throttle;              // between 0 and 1
  double thrust_magnitude;
  double flowrate_fuel;
  double flowrate_oxi;
  double temperature;
  double pressure;

public:
  thruster();
  ~thruster();

  Vector3d thrust;

  // specialisations
  virtual std::string  get_name();
  virtual std::string  get_model();
  virtual std::string  get_description();
  virtual double       get_mass();
  virtual Vector3d     get_size();
  void attach(spacecraft *to_vessel);
  bool attach_panel(instrumentpanel *to_panel);
  bool attach_hull();
  bool attach_cabin();
  void remove();
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
  virtual void update();
  virtual void update_if_time();

  // thruster-specific functionality
  virtual double get_thrust_max();
  Vector3d get_thrust();
  double get_thrust_magnitude();
  double get_flowrate_fuel();
  double get_flowrate_oxi();
  double get_temperature();
  double get_pressure();
  virtual void update_thrust();
  virtual void update_flowrate_fuel();
  virtual void update_flowrate_oxi();
  virtual void update_temperature();
  virtual void update_pressure();
  virtual void update_throttle(double newthrottle);
};

#endif // THRUSTER_H_INCLUDED
