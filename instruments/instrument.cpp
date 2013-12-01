#include "instrument.h"
#include "spacecraft.h"

instrument::instrument() {
  /// Default constructor
}

instrument::~instrument() {
  /// Default destructor
}

void instrument::attach(spacecraft *to_vessel) {
  /// Attach this device to the specified ship
  if(!to_vessel) {
    std::cout << "ERROR: tried to attach instrument " << get_name() << " to null vessel." << std::endl;
    return;
  }
  vessel = to_vessel;
  vessel->devices.push_back(this);
  vessel->instruments.push_back(this);
}

void instrument::remove() {
  /// Remove this device from whatever it's attached to
  if(!vessel) {
    std::cout << "ERROR: tried to remove instrument " << get_name() << " which is already not attached to anything." << std::endl;
    return;
  }
  vessel = nullptr;
  // remove it from the list of the vessel's devices
  //vessel->devices.erase(std::remove(vessel->devices.begin(), vessel->devices.end(), this), vessel->devices.end());
  //vessel->instruments.erase(std::remove(vessel->instruments.begin(), vessel->devices.end(), this), vessel->instruments.end());
  vessel->devices.remove(this);
  vessel->instruments.remove(this);
  // make sure there are no dangling connections
  for(auto &it : vessel->devices) {
    for(unsigned int i = 0; i != it->get_port_in_count(); ++i) {
      if(it->ports_in[i].target == this) {
        it->disconnect(i);
      }
    }
  }
}

//void instrument::update() {
//  /// Update the readings on this instrument and re-cache new values
//}
