//#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/base_units/imperial/mile.hpp>
#include "universe.h"
#include "starsystem.h"
#include "star.h"
#include "planet.h"
#include "astronaut.h"

universe extern root;
astronaut extern *player;

void init_universe() {
  std::cout << "Initialising universe..." << std::endl;

  starsystem *solarsystem = new starsystem;
  solarsystem->set_name("Solar System");
  root.starsystems.push_back(solarsystem);
  root.currentsystem = solarsystem;

  auto *sun = new star;
  sun->set_name("Sun");
  sun->set_designation("Sol");
  sun->set_mass(1989100000000000000000000000000.0);       // 1.9891 * 10^30 kg
  sun->set_radius(696342000.0);                           // 6.96342 * 10^5 m
  solarsystem->bodies.push_front(sun);
  solarsystem->primary = sun;

  // H = Mercury (Hermes)
  // V = Venus
  // E = Earth
  // M = Mars
  // J = Jupiter
  // S = Saturn
  // U = Uranus
  // N = Neptune

  auto *venus = new planet;
  venus->set_name("Venus");
  venus->set_designation("V");
  venus->set_mass(4867000000000000000000000.0);             // 4.867 * 10^24 kg
  venus->set_radius(6051900.0);                             // 6.0519 * 10^6 m
  venus->position.z = 108300000000.0;                       // 1.083 * 10^11 m
  venus->velocity.x = 35000.0;                              // 35000 m/s
  //venus->parent = sun;
  solarsystem->bodies.push_front(venus);

  auto *earth = new planet;
  earth->set_name("Earth");
  earth->set_designation("E");
  earth->set_mass(5972000000000000000000000.0);             // 5.972 * 10^24 kg
  earth->set_radius(6367500.0);                             // 6.3675 * 10^6 m
  earth->position.z = 147700000000.0;                       // 1.477 * 10^11 m
  //earth->velocity.x = 29800.0;                              // 29800 m/s
  double const degpersec = 360 / (23.934472 * 60 * 60);     // period of 23.934472 hours
  earth->spin = Quatd::fromEulerAngles(0, degpersec, 0);
  //earth->parent = sun;
  solarsystem->bodies.push_front(earth);

  player = new astronaut;
  player->set_name("Commander Jameson");
  //player->position = earth->position + Vector3d(0.0, earth->get_radius(), 0.0);
  player->position = earth->position + Vector3d(0.0, 0.0, earth->get_radius());
  player->velocity = earth->velocity;
  player->velocity.x += 7909.305;
  player->spin = earth->spin;
  player->state = astronaut::statetype::SURFACE;
  player->walking_on = (planet*)earth;
  root.astronauts.push_front(player);
  solarsystem->bodies.push_front(player);

  astronaut *player2 = new astronaut;
  player2->set_name("Rapid Space Dude");
  // two ways of doing the conversion:
  //double height = boost::units::quantity<boost::units::si::length>(100.0 * boost::units::imperial::mile_base_unit::unit_type()) / boost::units::si::meter;
  double height = 100.0 * boost::units::conversion_factor(boost::units::imperial::mile_base_unit::unit_type(), boost::units::si::meter);
  player2->position = earth->position + Vector3d(0.0, earth->get_radius() + height, 0.0);
  player2->velocity = earth->velocity;
  //player2->velocity.x += 100000000.0 / 3600 * boost::units::conversion_factor(boost::units::imperial::mile_base_unit::unit_type(), boost::units::si::meter);
  player2->velocity.x += 10000;
  player2->spin = earth->spin;
  player2->state = astronaut::statetype::SURFACE;
  player2->walking_on = earth;
  root.astronauts.push_front(player2);
  solarsystem->bodies.push_front(player2);

  for(auto  const &it : solarsystem->bodies) {
    std::cout << "  Accel due to gravity at surface of " << it->get_name() << " (" << it->get_designation() << ") is " << it->get_gravity_accel_surface() << std::endl;
  }

  std::cout << "Earth is at " << earth->position << std::endl;

  for(auto const &it : root.astronauts) {
    std::cout << "  Astronaut " << it->get_name() << " is standing on " << it->walking_on->get_name() << std::endl;
    std::cout << "    at            " << it->position << std::endl;
    std::cout << "    travelling at " << it->velocity << std::endl;
    //std::cout << "    spinning at   " << it->spin << std::endl;
    std::cout << "    above the surface by " << Vector3d(it->position - it->walking_on->position).length() - it->walking_on->get_radius() << "m" << std::endl;
    std::cout << "    feeling gravitational acceleration " << it->walking_on->get_gravity_accel(it->position, it->velocity) << "m/s^2" << std::endl;
  }

  earth->orbit.semimajor_axis     = 149597890000;               // a, metres
  earth->orbit.eccentricity       = 0.016710220;                // e
  earth->orbit.inclination        = 0.0000009;                  // i, rad
  earth->orbit.longitude_asc_node = -0.1965352;                 // o or omega, rad
  earth->orbit.argument_periapsis = 1.9933027;                  // w or omicron, rad
  earth->orbit.mean_anomaly_epoch = 5.612;                      // m or Mo, rad

  std::cout << "ORBIT: earth's periapsis      " << earth->get_periapsis() << std::endl;
  std::cout << "ORBIT: earth's apoapsis       " << earth->get_apoapsis() << std::endl;
  std::cout << "ORBIT: earth's orbital_period " << earth->get_orbital_period() << " (" << earth->get_orbital_period() / (60 * 60 * 24) << " days)" << std::endl;
  //std::cout << "ORBIT: mean_motion    " << earth->get_mean_motion() << std::endl;

  std::cout << "Universe initialised." << std::endl;
}
