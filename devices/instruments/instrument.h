#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

#include "vmath.h"
#include "device.h"

class instrumentpanel;

class instrument : public device {
  /// A device that can only be attached to an instrument panel
public:
  instrument();
  ~instrument();

  bool attach_cabin(instrumentpanel *to_panel);
  bool attach_hull();
};

#endif // INSTRUMENT_H_INCLUDED
