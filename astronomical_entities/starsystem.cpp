#include "starsystem.h"

starsystem::starsystem() {
  /// Default constructor
}

starsystem::~starsystem() {
  /// Default destructor
}

std::string starsystem::get_name() {
  if(name.size() != 0) {
    return name;
  } else {
    // random star name
    random_reset();                                 // reset the generator to its seed
    std::stringstream randomname;
    return get_random_name_ancient();
  }
}

void starsystem::set_name(std::string newname) {
  name = newname;
}

std::string starsystem::get_type() {
  /// Return the type of object this is
  return "star system";
}

std::string starsystem::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    // generate a random catalogue designation by some catalogue style i.e.
    // HE1327-2326 or R136a1 or Westerlund 1 BKS AS or W1-26
    // see http://en.wikipedia.org/wiki/Star_catalogue#Full-sky_catalogues
    random_reset();                                 // reset the generator to its seed
    std::stringstream designation;
    designation << get_random_char_alpha_upper()    // cluster
                << get_random_uint(1, 5000)         // cluster number
                << "-"
                << get_random_uint(1, 200);         // discovery number?
    return designation.str();
  }
}

void starsystem::set_designation(std::string newdesignation) {
  designation = newdesignation;
}

std::string starsystem::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    // describe it verbally based on its known traits:
    // distance, magnitude, colours, age, planets
    // TODO
    return "A star system.";
  }
}

void starsystem::set_description(std::string newdescription) {
  description = newdescription;
}
