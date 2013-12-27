#include "dustcloud.h"

dustcloud::dustcloud() {
  /// Default constructor
}

dustcloud::~dustcloud() {
  /// Default destructor
}

std::string dustcloud::get_name() {
  if(name.size() != 0) {
    return name;
  } else {
    std::stringstream randomname;
    randomname << "Unnamed Dust Cloud";
    return randomname.str();
  }
}

std::string dustcloud::get_type() {
  /// Return the type of object this is
  return "dust cloud";
}

std::string dustcloud::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    random_reset();                                 // reset the generator to its seed
    std::stringstream designation;
    designation <<  "DC" << get_random_uint(10000, 999999);
    return designation.str();
  }
}

std::string dustcloud::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    return "A sparse cloud of fine dust, composed of metallic elements, ice and long-chain hydrocarbons.  Perhaps it used to be something, once...";
  }
}

double dustcloud::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // make up a radius for this based on what's likely for the mass, rate of expansion and time since creation
    // TODO
    return 100000;
  }
}
