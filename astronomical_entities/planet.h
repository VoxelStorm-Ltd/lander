#ifndef PLANET_H_INCLUDED
#define PLANET_H_INCLUDED

#include "orbitalbody.h"

class planet : public orbitalbody {
  /// An astronomical object large enough to be rounded by its own gravity but not to undergo fusion
public:
  planet();
  ~planet();

  // getters and setters
  virtual double get_mass();
  virtual double get_radius();

  // render
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible();
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();
};

#endif // PLANET_H_INCLUDED
