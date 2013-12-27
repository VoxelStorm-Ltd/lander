#include "menu.h"
#include <iostream>

menu::menu() {
  /// Default constructor
}

menu::~menu() {
  /// Default destructor
}

std::string menu::get_name() {
  /// Return the name of whatever object this menu is attached to
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

std::string menu::get_type() {
  /// Return a descriptive type for whatever the object is this menu is attached to
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}

std::string menu::get_description() {
  /// Return a verbose text description of the object this menu is attached to
  // virtual placeholder
  std::cout << "WARNING: virtual function " << __PRETTY_FUNCTION__ << " called without specialisation - this should never happen." << std::endl;
  return "";
}
