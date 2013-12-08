#ifndef SPACECRAFT_H_INCLUDED
#define SPACECRAFT_H_INCLUDED

#include <list>
#include "physicsbody.h"

class device;
class astronaut;
class instrumentpanel;

class spacecraft : public physicsbody {
private:
  double temperature_hull;                // temperatures in kelvin
  double temperature_cabin;

public:
  std::list<device*> devices;             // all devices on-board
  std::list<device*> devices_hull;        // subset of devices out on the hull
  std::list<device*> devices_cabin;       // subset of devices mounted inside the cabin
  std::list<instrumentpanel*> panels;     // instrument panels in the cockpit
  std::list<astronaut*> occupants;        // who's on board

  // temporary navigation target
  Vector3d target;

  spacecraft();
  ~spacecraft();

  // getters and setters
  std::string get_name();
  std::string get_designation();
  std::string get_description();
  double get_mass();
  double get_radius();
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
