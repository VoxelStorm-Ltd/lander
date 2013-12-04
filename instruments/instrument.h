#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

#include "vmath.h"
#include "device.h"

class instrument : public device {
public:
  Vector3d position;                  // its location on the instrument panel, in metres
  Vector3d size;                      // how much physical space it takes up, in metres
  Quatd rotation;                     // its rotation relative to the panel

  instrument();
  ~instrument();

  virtual void attach(spacecraft *to_vessel);
  virtual void remove();

  //virtual void update();
  virtual void render();
};

#endif // INSTRUMENT_H_INCLUDED
