#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

#include "vectorstorm/vectorstorm.h"
#include "device.h"

class instrumentpanel;

class instrument : public device {
  /// A device that can only be attached to an instrument panel
protected:                                                                      // prevent this class being instantiated directly
  instrument();
public:
  virtual ~instrument();

  virtual std::string get_type() override final;
  bool attach_cabin(instrumentpanel *to_panel);
  bool attach_hull() override final;
};

#endif // INSTRUMENT_H_INCLUDED
