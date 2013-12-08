#ifndef DEVICE_EXTERNAL_H_INCLUDED
#define DEVICE_EXTERNAL_H_INCLUDED

#include "device.h"

class device_external : public device {
  /// A device that can only be attached to the hull on an eva
public:
  device_external();
  ~device_external();

  bool attach_panel(instrumentpanel *to_panel);
  bool attach_cabin();
};

#endif // DEVICE_EXTERNAL_H_INCLUDED
