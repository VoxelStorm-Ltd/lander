#ifndef THRUSTER_H_INCLUDED
#define THRUSTER_H_INCLUDED

#include "device_external.h"

class thruster : public device_external {
private:
  double throttle;                                                              // between 0 and 1
  double thrust_magnitude;
  double flowrate_fuel;
  double flowrate_oxi;
  double temperature;
  double pressure;

public:
  thruster();
  virtual ~thruster();

  vector3d thrust;

  // specialisations
  virtual std::string get_name() override final;
  virtual std::string get_model() override final;
  virtual std::string get_description() override final;
  virtual double      get_mass() override final;
  virtual vector3d    get_size() override final;
  bool attach_hull() override final;
  void remove_hull() override final;
  unsigned int get_port_in_count() override final;
  std::string  get_port_in_name(        unsigned int port) override final;
  std::string  get_port_in_description( unsigned int port) override final;
  bool         get_port_in_required(    unsigned int port) override final;
  unsigned int get_port_out_count() override final;
  std::string  get_port_out_name(       unsigned int port) override final;
  std::string  get_port_out_description(unsigned int port) override final;
  double       get_port_out_data(       unsigned int port) override final;
  virtual void update() override final;
  virtual void update_if_time() override final;

  // thruster-specific functionality
  virtual double get_thrust_max();
  vector3d const &get_thrust();
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
