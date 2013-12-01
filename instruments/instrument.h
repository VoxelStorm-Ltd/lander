#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

#include "vmath.h"
#include "device.h"

class instrument : public device {
public:
  Vector2d position;                  // its location on the instrument panel
  Vector2d size;                      // how much space it takes up on the panel

  instrument();
  ~instrument();

  //virtual void update();
  virtual void render();
};

#endif // INSTRUMENT_H_INCLUDED
