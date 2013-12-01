#ifndef SPACECRAFT_H_INCLUDED
#define SPACECRAFT_H_INCLUDED

#include <list>
#include "physicsbody.h"
#include "device.h"
#include "instrument.h"
#include "astronaut.h"

class spacecraft : public physicsbody {
private:
  double temperature_hull;                // temperatures in kelvin
  double temperature_cabin;

public:
  std::list<device*> devices;             // all devices on-board
  std::list<instrument*> instruments;     // those devices that are instruments
  std::list<astronaut*> occupants;        // who's on board

  // temporary navigation target
  Vector3d target;

  spacecraft();
  ~spacecraft();

  // getters and setters
  double get_temperature_hull();
  double get_temperature_cabin();

  virtual void update_state(double time, double deltatime);
  virtual void destroy();

  // rendering
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible();
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();
  void render_cockpit();
};

#endif // SPACECRAFT_H_INCLUDED
