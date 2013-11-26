#include "pulsar.h"

pulsar::pulsar()
  : period(0.0) {
  /// Default constructor
  period = get_random_double(0.5, 8.0);
}

pulsar::~pulsar() {
  /// Default destructor
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
