#ifndef SPACECRAFT_H_INCLUDED
#define SPACECRAFT_H_INCLUDED

#include <list>
#include "physicsbody.h"
#include "device.h"
#include "instrument.h"

class spacecraft : public physicsbody {
private:
  double temperature_hull;              // temperatures in kelvin
  double temperature_cabin;

public:
  std::list<device*> devices;
  std::list<instrument*> instruments;

  spacecraft();
  ~spacecraft();

  // getters and setters
  double get_temperature_hull();
  double get_temperature_cabin();

  virtual void update_state(double time, double deltatime);

  // rendering
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible();
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();
  void render_cockpit();
};

#endif // SPACECRAFT_H_INCLUDED
