#include "astronaut.h"
#include "spacecraft.h"

extern astronaut *player;

astronaut::astronaut()
  : state(statetype::INACTIVE),
    vessel_in(nullptr),
    walking_on(nullptr) {
  /// Default constructor
  set_mass(0.0);
  set_radius(0.0);
}

astronaut::~astronaut() {
  /// Default destructor
}

double astronaut::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, take a potshot at one around 70kg, plus ~10kg for a spacesuit
    random_reset();
    return get_random_double(70.0, 90.0);
  }
}
double astronaut::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // take a guess at radius (aka half height) based on human mass ~ 70kg
    // assume spacesuit weighs 10kg
    double const nakedmass = get_mass() - 10.0;
    // adjust range 60kg to 1.65m and 90kg to 1.84m
    double const height = (((nakedmass - 60.0) / (90.0 - 60.0) * (1.84 - 1.65)) + 1.65);
    return height * 0.550;   // average height * average centre of gravity ratio
  }
}

void astronaut::enter_ship(spacecraft *ship) {
  /// Put the astronaut in the specified ship
  state = statetype::IN_VESSEL;
  vessel_in = ship;
  walking_on = nullptr;
  position.assign();      // clear - assign defaults to 0
  velocity.assign();
  vessel_in->occupants.push_back(this);
}

void astronaut::exit_ship() {
  /// Exit the current ship, if any
  if(state != statetype::IN_VESSEL) {
    std::cout << "WARNING: astronaut " << get_name() << " told to exit ship while not in one, state is " << static_cast<int>(state) << std::endl;
    return;
  }
  position = vessel_in->position;
  velocity = vessel_in->velocity;
  vessel_in->occupants.remove(this);
  vessel_in = nullptr;
  // TODO: decide if we're on the ground or in space or what
  state = statetype::EVA;
}

void astronaut::kill() {
  /// Kill this astronaut, and perform whatever consequences are required
  std::cout << get_name() << " has been killed." << std::endl;
  state = statetype::DEAD;
  // check if it's the player
  if(this == player) {
    std::cout << "Congratulations!  You died." << std::endl;
  }
}
