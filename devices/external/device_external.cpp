#include "device_external.h"
#include <iostream>

device_external::device_external() {
  /// Default constructor
}

device_external::~device_external() {
  /// Default destructor
}

std::string device_external::get_type() {
  /// Return the type of object this is
  return "device (external)";
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

void device_external::remove_panel() {
  /// Remove this instrument from its panel only (leave it attached to any ship)
  // noop
  std::cout << "ERROR: tried to remove device_external " << get_name() << " from a panel (where it can't be mounted)." << std::endl;
}

void device_external::remove_cabin() {
  /// Remove this device from the cabin only (leave it attached to any ship)
  // noop
  std::cout << "ERROR: tried to remove device_external " << get_name() << " from the cabin (where it can't be mounted)." << std::endl;
}
