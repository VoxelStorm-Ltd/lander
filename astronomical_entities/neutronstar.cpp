#include "neutronstar.h"

neutronstar::neutronstar() {
  /// Default constructor
}

neutronstar::~neutronstar() {
  /// Default destructor
}

std::string neutronstar::get_type() {
  /// Return the type of object this is
  return "neutron star";
}

std::string neutronstar::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    return "A neutron star.  This is a stellar remnant composed almost entirely of neutrons.";
  }
}
