//#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/base_units/imperial/mile.hpp>
#include "universe.h"
#include "starsystem.h"
#include "star.h"
#include "planet.h"
#include "moon.h"
#include "astronaut.h"
#include "spacecraft.h"
#include "instrumentpanel.h"
#include "altimeter.h"
#include "sensor_pressure.h"
#include "display.h"
#include "display_digital.h"
#include "display_small.h"
#include "mapper_system.h"
#include "memory.h"
#include "display_converter_analogue_digital.h"

universe extern root;
astronaut extern *player;

// DEBUG ONLY:
memory *mem_zoom;
device *picktestdevice;

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
  solarsystem->bodies.push_back(sun);
  solarsystem->primary = sun;

  auto *mercury = new planet;
  mercury->set_name("Mercury");
  mercury->set_designation("H");                              // aka Hermes
  mercury->set_mass(330220000000000000000000.0);              // 3.3022 * 10^23 kg
  mercury->set_radius(2439700.0);                             // 2439.7 km
  mercury->parent = sun;
  mercury->orbit.semimajor_axis     = 57909100000.0;
  solarsystem->bodies.push_back(mercury);

  auto *venus = new planet;
  venus->set_name("Venus");
  venus->set_designation("V");
  venus->set_mass(4867000000000000000000000.0);             // 4.867 * 10^24 kg
  venus->set_radius(6051900.0);                             // 6.0519 * 10^6 m
  //venus->position.z = 108300000000.0;                       // 1.083 * 10^11 m
  //venus->velocity.x = 35000.0;                              // 35000 m/s
  venus->parent = sun;
  venus->orbit.semimajor_axis     = 108208000000.0;
  solarsystem->bodies.push_back(venus);

  auto *earth = new planet;
  earth->set_name("Earth");
  earth->set_designation("E");
  earth->set_mass(5972000000000000000000000.0);             // 5.972 * 10^24 kg
  earth->set_radius(6367500.0);                             // 6.3675 * 10^6 m
  //earth->position.z = 147700000000.0;                       // 1.477 * 10^11 m
  //earth->velocity.x = 29800.0;                              // 29800 m/s
  double const degpersec = 360 / (23.934472 * 60 * 60);     // period of 23.934472 hours
  earth->spin = Quatd::fromEulerAngles(0, degpersec, 0);
  earth->parent = sun;
  //earth->orbit.semimajor_axis     = 149597890000;               // a, metres
  //earth->orbit.eccentricity       = 0.016710220;                // e
  //earth->orbit.inclination        = 0.0000009;                  // i, rad
  //earth->orbit.longitude_asc_node = -0.1965352;                 // o or omega, rad
  //earth->orbit.argument_periapsis = 1.9933027;                  // w or omicron, rad
  //earth->orbit.mean_anomaly_epoch = 5.612;                      // m or Mo, rad
  earth->orbit.semimajor_axis     = 147700000000.0;     // not actually correct
  earth->orbit.eccentricity       = 0.0;                // not actually correct
  earth->orbit.inclination        = 0.0;                // not actually correct
  earth->orbit.longitude_asc_node = 0.0;                // not actually correct
  earth->orbit.argument_periapsis = 0.0;                // not actually correct
  earth->orbit.mean_anomaly_epoch = 0.0;                // not actually correct
  solarsystem->bodies.push_back(earth);

  auto *luna = new moon;
  luna->set_name("Moon");
  luna->set_designation("Luna");
  luna->set_mass(73477000000000000000000.0);               // 7.3477 * 10^22 kg
  luna->set_radius(1738140.0);                             // 1738.14km
  luna->spin = Quatd::fromEulerAngles(0, 360 / (27.321582 * 60 * 60 * 24), 0); // period of 27.321582 days
  luna->parent = earth;
  luna->orbit.semimajor_axis     = 384399000.0;
  solarsystem->bodies.push_back(luna);

  auto *mars = new planet;
  mars->set_name("Mars");
  mars->set_designation("M");
  mars->set_mass(641850000000000000000000.0);              // 6.4185 * 10^23 kg
  mars->set_radius(3396200.0);                             // 3396.2 km
  mars->parent = sun;
  mars->orbit.semimajor_axis = 227939100000.0;
  solarsystem->bodies.push_back(mars);

  auto *jupiter = new planet;
  jupiter->set_name("Jupiter");
  jupiter->set_designation("J");
  jupiter->set_mass(1898600000000000000000000000.0);       // 1.8986 * 10^27 kg
  jupiter->set_radius(71492000.0);                         // 71492 km
  jupiter->parent = sun;
  jupiter->orbit.semimajor_axis = 778547200000.0;
  solarsystem->bodies.push_back(jupiter);

  auto *saturn = new planet;
  saturn->set_name("Saturn");
  saturn->set_designation("S");
  saturn->set_mass(568460000000000000000000000.0);        // 5.6846 * 10^26 kg
  saturn->set_radius(60268000.0);                         // 60268 km
  saturn->parent = sun;
  saturn->orbit.semimajor_axis = 1433449370000.0;
  solarsystem->bodies.push_back(saturn);

  auto *uranus = new planet;
  uranus->set_name("Uranus");
  uranus->set_designation("U");
  uranus->set_mass(86810000000000000000000000.0);         // 18.6810 * 10^25 kg
  uranus->set_radius(25559000.0);                         // 25559 km
  uranus->parent = sun;
  uranus->orbit.semimajor_axis = 2876679082000.0;
  solarsystem->bodies.push_back(uranus);

  auto *neptune = new planet;
  neptune->set_name("Neptune");
  neptune->set_designation("N");
  neptune->set_mass(102430000000000000000000000.0);        // 1.0243 * 10^26 kg
  neptune->set_radius(24764000.0);                         // 24764 km
  neptune->parent = sun;
  neptune->orbit.semimajor_axis = 4503443661000.0;
  solarsystem->bodies.push_back(neptune);

  for(auto const &it : solarsystem->bodies) {
    it->update_state(-2.0, 1.0);         // fill in back-history for the solar system to get an accurate velocity at t=0
    it->update_state(-1.0, 1.0);
  }

  astronaut *groundguy = new astronaut;
  groundguy->set_name("Usain Bolt");        // travelling at escape velocity, innit
  groundguy->position = earth->position + Vector3d(0.0, 0.0, earth->get_radius());
  groundguy->velocity = earth->velocity;
  groundguy->velocity.x += 7909.305;
  groundguy->spin = earth->spin;
  groundguy->state = astronaut::statetype::SURFACE;
  groundguy->walking_on = (planet*)earth;
  root.astronauts.push_back(groundguy);
  solarsystem->bodies.push_back(groundguy);

  astronaut *player2 = new astronaut;
  player2->set_name("Major Tom");
  player2->state = astronaut::statetype::EVA;
  // two ways of doing the conversion:
  //double height = boost::units::quantity<boost::units::si::length>(100.0 * boost::units::imperial::mile_base_unit::unit_type()) / boost::units::si::meter;
  double height = 100.0 * boost::units::conversion_factor(boost::units::imperial::mile_base_unit::unit_type(), boost::units::si::meter);
  player2->position = earth->position + Vector3d(0.0, earth->get_radius() + height, 0.0);
  player2->velocity = earth->velocity;
  //player2->velocity.x += 100000000.0 / 3600 * boost::units::conversion_factor(boost::units::imperial::mile_base_unit::unit_type(), boost::units::si::meter);
  player2->velocity.x += 10000;
  player2->spin = earth->spin;
  player2->state = astronaut::statetype::EVA;
  player2->walking_on = earth;
  root.astronauts.push_back(player2);
  solarsystem->bodies.push_back(player2);

  player = new astronaut;
  player->set_name("Commander Jameson");
  root.astronauts.push_back(player);

  spacecraft *playership = new spacecraft;
  playership->set_name("Cobra Mk3");
  solarsystem->bodies.push_back(playership);
  player->enter_ship(playership);
  playership->position = earth->position + Vector3d(earth->get_radius() + 370000, 0.0, 0.0);    // ~= ISS altitude
  playership->velocity = earth->velocity + Vector3d(0.0, 0.0, 7710.0);                          // ~= ISS speed
  instrumentpanel *controlpanel = new instrumentpanel;
  controlpanel->size = Vector3d(2.0, 1.0, 0.5);
  controlpanel->position = Vector3d(-controlpanel->size.x / 2.0, 0.6, -0.3);  // ~300mm in front of user
  controlpanel->rotation = Quatd::fromAxisRot(Vector3d(1.0, 0.0, 0.0), -60.0);
  controlpanel->attach(playership);

  altimeter *test_altimeter = new altimeter;
  sensor_pressure *test_pressuresensor = new sensor_pressure;
  test_altimeter->attach(playership);
  test_altimeter->attach_panel(controlpanel);
  test_altimeter->set_position(0.1, 0.84, 0.0);
  test_pressuresensor->attach(playership);
  test_pressuresensor->attach_hull();
  test_altimeter->connect(0, test_pressuresensor, 0);
  test_altimeter->describe_to_console();

  display *mainmonitor = new display;
  mapper_system *mainmapper = new mapper_system;
  ///memory *mem_zoom = new memory;
  mem_zoom = new memory;
  memory *mem_planetref = new memory;
  mainmonitor->attach(playership);
  mainmonitor->attach_panel(controlpanel);
  mainmonitor->set_position(0.45, 0.5, 0.0);
  mainmapper->attach(playership);
  mainmapper->attach_panel(controlpanel);
  mainmapper->set_position(0.9, 0.2, 0.0);
  mainmonitor->connect(0, mainmapper, 0);
  mem_zoom->attach(playership);
  mem_zoom->attach_panel(controlpanel);
  mem_zoom->set_position(0.9, 0.05, 0.0);
  mem_zoom->set_memory_value(0.00001) ;                   // set the scale ratio
  mainmapper->connect(0, mem_zoom, 0);                    // hook it up to the zoom reference frame input
  mem_planetref->attach(playership);
  mem_planetref->attach_panel(controlpanel);
  mem_planetref->set_position(1.1, 0.05, 0.0);
  mem_planetref->set_memory_value(4.0);                   // earth = 4
  mainmapper->connect(3, mem_planetref, 0);               // hook it up to the trails reference frame input
  mainmonitor->describe_to_console();
  mainmapper->describe_to_console();
  mem_planetref->describe_to_console();

  display_digital *staticmonitor1 = new display_digital;
  staticmonitor1->attach(playership);
  staticmonitor1->attach_panel(controlpanel);
  staticmonitor1->set_position(1.05, 0.5, 0.0);
  //staticmonitor1->connect(0, test_pressuresensor, 0);             // noise source
  //staticmonitor1->connect(0, mainmapper, 0);
  display_small *staticmonitor2 = new display_small;
  staticmonitor2->attach(playership);
  staticmonitor2->attach_panel(controlpanel);
  staticmonitor2->set_position(0.3, 0.84, 0.0);
  staticmonitor2->connect(0, test_pressuresensor, 0);             // noise source
  //staticmonitor2->connect(0, mainmapper, 0);

  display_converter_analogue_digital *converter1 = new display_converter_analogue_digital;
  converter1->attach(playership);
  converter1->attach_panel(controlpanel);
  converter1->set_position(1.2, 0.38, 0.0);
  converter1->connect(0, mainmapper, 0);
  staticmonitor1->connect(0, converter1, 0);

  spacecraft *crashtester = new spacecraft;
  crashtester->set_name("Crash Tester");
  solarsystem->bodies.push_back(crashtester);
  crashtester->position = earth->position + Vector3d(earth->get_radius() + 30000000, 0.0, earth->get_radius());
  crashtester->velocity = earth->velocity + Vector3d(-20000.0, 0.0, 0.0);


  for(auto  const &it : solarsystem->bodies) {
    std::cout << "  Accel due to gravity at surface of " << it->get_name() << " (" << it->get_designation() << ") is " << it->get_gravity_accel_surface() << std::endl;
  }

  std::cout << "Earth is at " << earth->position << std::endl;

  for(auto const &it : root.astronauts) {
    std::cout << "  Astronaut " << it->get_name();
    switch(it->state) {
    case astronaut::statetype::IN_VESSEL:
      std::cout << " is aboard " << it->vessel_in->get_name() << " (" << it->vessel_in->get_designation() << ")" << std::endl;
      break;
    case astronaut::statetype::SURFACE:
      std::cout << " is standing on " << it->walking_on->get_name() << std::endl;
      break;
    case astronaut::statetype::EVA:
      std::cout << " is floating in space" << std::endl;
      break;
    case astronaut::statetype::ATMOSPHERIC:
      std::cout << " is falling through an atmosphere over " << it->walking_on->get_name() << std::endl;
      std::cout << "    above the surface by " << Vector3d(it->position - it->walking_on->position).length() - it->walking_on->get_radius() << "m" << std::endl;
      std::cout << "    feeling gravitational acceleration " << it->walking_on->get_gravity_accel(it->position, it->velocity) << "m/s^2" << std::endl;
      break;
    default:
      std::cout << std::endl;
      break;
    }
    std::cout << "    at            " << it->position << std::endl;
    std::cout << "    travelling at " << it->velocity << std::endl;
    //std::cout << "    spinning at   " << it->spin << std::endl;
  }

  std::cout << "ORBIT: earth's periapsis      " << earth->get_periapsis() << std::endl;
  std::cout << "ORBIT: earth's apoapsis       " << earth->get_apoapsis() << std::endl;
  std::cout << "ORBIT: earth's orbital_period " << earth->get_orbital_period() << " (" << earth->get_orbital_period() / (60 * 60 * 24) << " days)" << std::endl;
  //std::cout << "ORBIT: mean_motion    " << earth->get_mean_motion() << std::endl;

  std::cout << "Universe initialised." << std::endl;
}
