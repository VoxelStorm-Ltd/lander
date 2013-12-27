#ifndef DEVICE_EXTERNAL_H_INCLUDED
#define DEVICE_EXTERNAL_H_INCLUDED

#include "device.h"

class device_external : public device {
  /// A device that can only be attached to the hull on an eva
protected:
  device_external();                     // prevent this class being instantiated directly
public:
  virtual ~device_external();

  virtual std::string get_type();
  bool attach_panel(instrumentpanel *to_panel);
  bool attach_cabin();
  void remove_panel();
  void remove_cabin();
};

#endif // DEVICE_EXTERNAL_H_INCLUDED
