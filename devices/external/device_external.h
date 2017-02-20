#ifndef DEVICE_EXTERNAL_H_INCLUDED
#define DEVICE_EXTERNAL_H_INCLUDED

#include "device.h"

class device_external : public device {
  /// A device that can only be attached to the hull on an eva
protected:
  device_external();                                                            // prevent this class being instantiated directly
public:
  virtual ~device_external();

  virtual std::string get_type() override final;
  bool attach_panel(instrumentpanel *to_panel) override final;
  bool attach_cabin() override final;
  void remove_panel() override final;
  void remove_cabin() override final;
};

#endif // DEVICE_EXTERNAL_H_INCLUDED
