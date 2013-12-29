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
#include "thruster.h"
#include "switch_onoff.h"
#include "button_momentary.h"
#include "led_green.h"
#include "telltale.h"
#include "operator_mul.h"
#include "operator_add.h"
#include "operator_sub.h"
#include "display_number.h"
#include "camera.h"
#include "terminal.h"
#include "computer_mission.h"

extern universe root;
extern astronaut *player;

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
  luna->orbit.semimajor_axis = 384399000.0;
  solarsystem->bodies.push_back(luna);

  auto *mars = new planet;
  mars->set_name("Mars");
  mars->set_designation("M");
  mars->set_mass(641850000000000000000000.0);              // 6.4185 * 10^23 kg
  mars->set_radius(3396200.0);                             // 3396.2 km
  mars->parent = sun;
  mars->orbit.semimajor_axis = 227939100000.0;
  solarsystem->bodies.push_back(mars);

  auto *phobos = new moon;
  phobos->set_name("Phobos");
  phobos->set_designation("Mars I");
  phobos->set_mass(10659000000000000.0);                   // 1.0659 * 10^16 kg
  phobos->set_radius(11266.7);                             // 11.2667km
  phobos->parent = mars;
  phobos->orbit.semimajor_axis = 9376000.0;
  solarsystem->bodies.push_back(phobos);

  auto *deimos = new moon;
  deimos->set_name("Deimos");
  deimos->set_designation("Mars II");
  deimos->set_mass(1476200000000000.0);                    // 1.4762 * 10^15 kg
  deimos->set_radius(6200);                                // 6.2km
  deimos->parent = mars;
  deimos->orbit.semimajor_axis = 23463200.0;
  solarsystem->bodies.push_back(deimos);

  auto *jupiter = new planet;
  jupiter->set_name("Jupiter");
  jupiter->set_designation("J");
  jupiter->set_mass(1898600000000000000000000000.0);       // 1.8986 * 10^27 kg
  jupiter->set_radius(71492000.0);                         // 71492 km
  jupiter->parent = sun;
  jupiter->orbit.semimajor_axis = 778547200000.0;
  solarsystem->bodies.push_back(jupiter);

  auto *io = new moon;
  io->set_name("Io");
  io->set_designation("Jupiter I");
  io->set_mass(89319000000000000000000.0);                // 8.9319 * 10^22 kg
  io->set_radius(1821300.0);                              // 1,821.3km
  io->parent = jupiter;
  io->orbit.semimajor_axis = 421700000.0;
  solarsystem->bodies.push_back(io);

  auto *europa = new moon;
  europa->set_name("Europa");
  europa->set_designation("Jupiter II");
  europa->set_mass(47998000000000000000000.0);            // 4.7998 * 10^22 kg
  europa->set_radius(1560800.0);                          // 1,560.8km
  europa->parent = jupiter;
  europa->orbit.semimajor_axis = 670900000.0;
  solarsystem->bodies.push_back(europa);

  auto *ganymede = new moon;
  ganymede->set_name("Ganymede");
  ganymede->set_designation("Jupiter III");
  ganymede->set_mass(148190000000000000000000.0);         // 1.4819 * 10^23 kg
  ganymede->set_radius(2634100.0);                        // 2,634.1km
  ganymede->parent = jupiter;
  ganymede->orbit.semimajor_axis = 1070400000.0;
  solarsystem->bodies.push_back(ganymede);

  auto *callisto = new moon;
  callisto->set_name("Callisto");
  callisto->set_designation("Jupiter IV");
  callisto->set_mass(107593800000000000000000.0);         // 1.4819 * 10^23 kg
  callisto->set_radius(2410300.0);                        // 2,634.1km
  callisto->parent = jupiter;
  callisto->orbit.semimajor_axis = 1882700000.0;
  solarsystem->bodies.push_back(callisto);

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
  //playership->position = europa->position + Vector3d(europa->get_radius() + 10000000, 0.0, 0.0);
  playership->velocity = earth->velocity + Vector3d(0.0, 0.0, 7710.0);                          // ~= ISS speed
  //playership->velocity = europa->velocity + Vector3d(0.0, 0.0, 10000.0);
  playership->rotation *= Quatd::fromAxisRot(Vector3d(1.0, 0.0, 0.0), 90.0);
  instrumentpanel *controlpanel = new instrumentpanel;
  controlpanel->size = Vector3d(2.0, 1.0, 0.5);
  controlpanel->position = Vector3d(-controlpanel->size.x / 2.0, 0.6, -0.3);  // ~300mm in front of user
  controlpanel->rotation = Quatd::fromAxisRot(Vector3d(1.0, 0.0, 0.0), -60.0);
  controlpanel->attach(playership);

  thruster *engine_main = new thruster;
  engine_main->attach(playership);
  engine_main->attach_hull();
  engine_main->set_position(0.0, -1.5, 0.0);
  switch_onoff *throttle = new switch_onoff;
  throttle->attach(playership);
  throttle->attach_panel(controlpanel);
  throttle->set_position(1.0, 0.5, 0.0);
  engine_main->connect(0, throttle, 0);
  throttle->bind_key(GLFW_KEY_T);
  led_green *engine_light = new led_green;
  engine_light->attach(playership);
  engine_light->attach_panel(controlpanel);
  engine_light->set_position(0.98, 0.5, 0.0);
  engine_light->connect(0, engine_main, 0);         // input to throttle value output of engine
  telltale *engine_telltale = new telltale;
  engine_telltale->attach(playership);
  engine_telltale->attach_panel(controlpanel);
  engine_telltale->set_position(0.98, 0.53, 0.0);
  engine_telltale->connect(0, engine_main, 0);      // input to throttle value output of engine

  altimeter *test_altimeter = new altimeter;
  sensor_pressure *test_pressuresensor = new sensor_pressure;
  test_altimeter->attach(playership);
  test_altimeter->attach_panel(controlpanel);
  test_altimeter->set_position(0.02, 0.84, 0.0);
  test_pressuresensor->attach(playership);
  test_pressuresensor->attach_hull();
  test_altimeter->connect(0, test_pressuresensor, 0);
  display_number *pressure_disp = new display_number;
  pressure_disp->attach(playership);
  pressure_disp->attach_panel(controlpanel);
  pressure_disp->set_position(test_altimeter->get_position() + Vector3d(0.0, -0.03, 0.0));
  pressure_disp->connect(0, test_pressuresensor, 0);

  display *mainmonitor = new display;
  mapper_system *mainmapper = new mapper_system;
  memory *mem_ref = new memory;
  mainmonitor->attach(playership);
  mainmonitor->attach_panel(controlpanel);
  mainmonitor->set_position(0.55, 0.5, 0.0);
  mainmapper->attach(playership);
  mainmapper->attach_panel(controlpanel);
  mainmapper->set_position(mainmonitor->get_position() + Vector3d(0.0, -(mainmapper->get_size().y + 0.02), 0.0));
  mainmonitor->connect(0, mainmapper, 0);

  // zoom system
  button_momentary *zoom_in = new button_momentary;
  zoom_in->attach(playership);
  zoom_in->attach_panel(controlpanel);
  zoom_in->set_position(mainmonitor->get_position() + Vector3d(mainmonitor->get_size().x - 0.02, -0.03, 0.0));
  button_momentary *zoom_out = new button_momentary;
  zoom_out->attach(playership);
  zoom_out->attach_panel(controlpanel);
  zoom_out->set_position(mainmonitor->get_position() + Vector3d(mainmonitor->get_size().x - 0.05, -0.03, 0.0));
  memory *mem_1 = new memory;
  mem_1->attach(playership);
  mem_1->attach_panel(controlpanel);
  mem_1->set_position(mainmapper->get_position() + Vector3d(mainmapper->get_size().x + 0.01, 0.0, 0.0));
  mem_1->set_memory_value(1.0);
  memory *mem_05 = new memory;
  mem_05->attach(playership);
  mem_05->attach_panel(controlpanel);
  mem_05->set_position(mem_1->get_position() + Vector3d(0.0, 0.01, 0.0));
  mem_05->set_memory_value(0.5);
  memory *mem_2 = new memory;
  mem_2->attach(playership);
  mem_2->attach_panel(controlpanel);
  mem_2->set_position(mem_05->get_position() + Vector3d(0.0, 0.01, 0.0));
  mem_2->set_memory_value(2.0);
  zoom_out->connect(0, mem_1, 0);                       // off value = 1
  zoom_out->connect(1, mem_05, 0);                      // on value = 1/2
  zoom_in->connect(0, zoom_out, 0);                     // off value = zoom out button
  zoom_in->connect(1, mem_2, 0);                        // on value = 2
  memory *mem_zoom = new memory;
  mem_zoom->attach(playership);
  mem_zoom->attach_panel(controlpanel);
  mem_zoom->set_position(mem_2->get_position() + Vector3d(0.0, 0.01, 0.0));
  mem_zoom->set_memory_value(0.00001);                  // set the initial scale ratio
  operator_mul *zoom_mul = new operator_mul;
  zoom_mul->attach(playership);
  zoom_mul->attach_panel(controlpanel);
  zoom_mul->set_position(mem_1->get_position() + Vector3d(0.03, 0.00, 0.0));
  zoom_mul->connect(0, zoom_in, 0);                     // input 1 = zoom in button
  zoom_mul->connect(1, mem_zoom, 0);                    // input 2 = last zoom value
  mem_zoom->connect(0, zoom_mul, 0);                    // zoom value updates from result
  mainmapper->connect(0, mem_zoom, 0);                  // hook it up to the zoom reference frame input
  zoom_in->bind_key(GLFW_KEY_EQUAL);
  zoom_out->bind_key(GLFW_KEY_MINUS);
  display_number *zoom_disp = new display_number;
  zoom_disp->attach(playership);
  zoom_disp->attach_panel(controlpanel);
  zoom_disp->set_position(zoom_out->get_position() + Vector3d(-0.105, 0.0, 0.0));
  zoom_disp->connect(0, mem_zoom, 0);

  // planet ref system
  mem_ref->attach(playership);
  mem_ref->attach_panel(controlpanel);
  mem_ref->set_position(mem_zoom->get_position() + Vector3d(0.0, 0.01, 0.0));
  mem_ref->set_memory_value(4.0);                 // earth = 4
  mainmapper->connect(3, mem_ref, 0);             // hook it up to the trails reference frame input
  button_momentary *ref_prev = new button_momentary;
  ref_prev->attach(playership);
  ref_prev->attach_panel(controlpanel);
  ref_prev->set_position(zoom_out->get_position() + Vector3d(0.0, -0.03, 0.0));
  button_momentary *ref_next = new button_momentary;
  ref_next->attach(playership);
  ref_next->attach_panel(controlpanel);
  ref_next->set_position(zoom_in->get_position() + Vector3d(0.0, -0.03, 0.0));
  operator_add *ref_add = new operator_add;
  ref_add->attach(playership);
  ref_add->attach_panel(controlpanel);
  ref_add->set_position(zoom_mul->get_position() + Vector3d(0.0, 0.015, 0.0));
  ref_add->connect(0, mem_ref, 0);                     // input 1 = last ref value
  ref_add->connect(1, ref_next, 0);                    // input 2 = next ref button
  operator_sub *ref_sub = new operator_sub;
  ref_sub->attach(playership);
  ref_sub->attach_panel(controlpanel);
  ref_sub->set_position(ref_add->get_position() + Vector3d(0.0, 0.015, 0.0));
  ref_sub->connect(0, ref_add, 0);                     // input 1 = result of incrementor
  ref_sub->connect(1, ref_prev, 0);                    // input 2 = prev ref button
  mem_ref->connect(0, ref_sub, 0);                     // ref value updates from result
  display_number *ref_disp = new display_number;
  ref_disp->attach(playership);
  ref_disp->attach_panel(controlpanel);
  ref_disp->set_position(ref_prev->get_position() + Vector3d(-0.105, 0.0, 0.0));
  ref_disp->connect(0, mem_ref, 0);

  display_digital *monitor_digital = new display_digital;
  monitor_digital->attach(playership);
  monitor_digital->attach_panel(controlpanel);
  monitor_digital->set_position(1.05, 0.5, 0.0);
  display_small *monitor_small = new display_small;
  monitor_small->attach(playership);
  monitor_small->attach_panel(controlpanel);
  monitor_small->set_position(test_altimeter->get_position() + Vector3d(0.0, -0.2, 0.0));
  monitor_small->connect(0, test_pressuresensor, 0);             // noise source

  display_converter_analogue_digital *converter1 = new display_converter_analogue_digital;
  converter1->attach(playership);
  converter1->attach_panel(controlpanel);
  converter1->set_position(1.2, 0.38, 0.0);
  converter1->connect(0, mainmapper, 0);
  monitor_digital->connect(0, converter1, 0);

  camera *cam1 = new camera;
  cam1->attach(playership);
  cam1->attach_hull();
  cam1->set_position(Vector3d(0.0, 0.0, 2.0));
  monitor_digital->connect(0, cam1, 0);

  display *monitor_computer = new display;
  monitor_computer->attach(playership);
  monitor_computer->attach_panel(controlpanel);
  monitor_computer->set_position(0.15, 0.5, 0.0);
  terminal *tty = new terminal;
  tty->attach(playership);
  tty->attach_panel(controlpanel);
  tty->set_position(monitor_computer->get_position() + Vector3d(0.0, -(tty->get_size().y + 0.02), 0.0));
  monitor_computer->connect(0, tty, 0);
  computer_mission *computer = new computer_mission;
  computer->attach(playership);
  computer->attach_panel(controlpanel);
  computer->set_position(tty->get_position() + Vector3d(0.0, -(computer->get_size().y + 0.02), 0.0));
  tty->connect(0, computer, 0);
  computer->connect(0, tty, 1);

  spacecraft *crashtester = new spacecraft;
  crashtester->set_name("Crash Tester");
  solarsystem->bodies.push_back(crashtester);
  crashtester->position = earth->position + Vector3d(earth->get_radius() + 30000000, 0.0, earth->get_radius());
  crashtester->velocity = earth->velocity + Vector3d(-20000.0, 1000.0, 0.0);
  crashtester->rotation *= Quatd::fromAxisRot(Vector3d(0.0, 0.0, 1.0), 90.0);

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
