#ifndef ALTIMETER_H_INCLUDED
#define ALTIMETER_H_INCLUDED

#include "instrument.h"

class altimeter : public instrument {
private:
  double altitude;

public:
  altimeter();
  ~altimeter();

  std::string get_name() override final;
  std::string  get_model() override final;
  std::string get_description() override final;
  unsigned int get_port_in_count() override final;
  std::string get_port_in_name(       unsigned int port) override final;
  std::string get_port_in_description(unsigned int port) override final;
  bool get_port_in_required(          unsigned int port) override final;
  Vector3d get_size() override final;
  void update() override final;
  void render() override final;
};

#endif // ALTIMETER_H_INCLUDED
