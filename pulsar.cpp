#include "pulsar.h"

pulsar::pulsar()
  : period(0.0) {
  /// Default constructor
  random_reset();
  period = get_random_double(0.5, 8.0);
}

pulsar::~pulsar() {
  /// Default destructor
}

std::string pulsar::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    // generate a random astronomical designation in the standard style "2003 VB12"
    // see http://en.wikipedia.org/wiki/Provisional_designation_in_astronomy
    random_reset();                                 // reset the generator to its seed
    std::stringstream designation;
    designation << "PSR "
                << get_random_uint(1926, 2059)      // year
                << " "
                << get_random_char_alpha_upper()    // month (allow Z since we're lazy)
                << get_random_char_alpha_upper()    // order of discovery letter
                << get_random_uint(1, 530);         // order of discovery number
    return designation.str();
  }
}

std::string pulsar::get_description() {
  if(description.size() != 0) {
    return description;
  } else {
    std::stringstream desc;
    desc << "A pulsar.  This is a highly magnetized, rotating neutron "
            "star that emits a beam of electromagnetic radiation.  This "
            "one pulses once every " << period << " seconds.";
    return desc.str();
  }
}
