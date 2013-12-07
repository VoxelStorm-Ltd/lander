#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

#include "vmath.h"
#include "device.h"
//#include "instrumentpanel.h"

class instrumentpanel;

class instrument : public device {
private:
  Vector3d position;                  // its location on the instrument panel, in metres

public:
  instrumentpanel *panel;             // what instrument panel it's attached to

  instrument();
  ~instrument();

  // getters and setters
  virtual Vector3d get_position();
  virtual Vector3d get_size();
  virtual Quatd    get_rotation();
  virtual void     set_position(Vector3d const &newposition);
  virtual void     set_position(double x, double y, double z);
  // no default setters for size and rotation - these are fixed for most instruments

  virtual void attach(spacecraft *to_vessel);
  virtual void attach(instrumentpanel *to_panel);
  virtual void remove();
  virtual void remove_panel();

  //virtual void update();
  virtual void render();
};

#endif // INSTRUMENT_H_INCLUDED
