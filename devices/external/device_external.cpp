#include "device_external.h"

device_external::device_external() {
  /// Default constructor
}

device_external::~device_external() {
  /// Default destructor
}

bool device_external::attach_panel(instrumentpanel *to_panel __attribute__((__unused__))) {
  /// Attempt to attach this device to an instrument panel, return success status
  // this must be mounted externally on the hull
  return false;
}

bool device_external::attach_cabin() {
  /// Attempt to attach this device to a wall of the cabin, return success status
  // this must be mounted externally on the hull
  return false;
}
