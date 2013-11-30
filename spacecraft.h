#ifndef SPACECRAFT_H_INCLUDED
#define SPACECRAFT_H_INCLUDED

#include "physicsbody.h"
#include "instrument.h"

class spacecraft : public physicsbody {
public:
  std::vector<instrument*> instruments;

  spacecraft();
  ~spacecraft();

  // rendering
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible();
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();
  void render_cockpit();
};

#endif // SPACECRAFT_H_INCLUDED
