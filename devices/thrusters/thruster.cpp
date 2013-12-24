#include "thruster.h"
#include "vmath.h"
#include "spacecraft.h"

thruster::thruster()
  : throttle(0.0),
    thrust_magnitude(0.0),
    flowrate_fuel(0.0),
    flowrate_oxi(0.0),
    temperature(290.0),   // ~16C
    pressure(101325.0) {  // 1Atm
  /// Default constructor
  ports_in.resize(get_port_in_count());     // anything with input ports needs this

  // rotate it to point down by default
  //rotation *= Quatd::fromEulerAngles(90.0, 0.0, 0.0);
  rotation *= Quatd::fromAxisRot(Vector3d(1.0, 0.0, 0.0), 90.0);
}

thruster::~thruster() {
  /// Default destructor
}

std::string thruster::get_name() {
  /// Return the name of this device
  return "thruster";
}

std::string thruster::get_model() {
  /// Return a model name for this device
  random_reset();
  return "TR-202";
}

std::string thruster::get_description() {
  /// Return a detailed description of this device
  std::stringstream ss;
  ss << "A variable throttle hypergolic chemical reaction rocket engine with a maximum thrust of "
     << get_thrust_max() << "N.";
  return ss.str();
}

double thruster::get_mass() {
  /// Return the weight of the device, in kilograms
  return 180.0;                       // based on https://en.wikipedia.org/wiki/Descent_Propulsion_System
}

Vector3d thruster::get_size() {
  /// Return a size for this object, in metres - hardcoded
  return Vector3d(1.5, 2.3, 1.5);     // based on https://en.wikipedia.org/wiki/Descent_Propulsion_System
}

bool thruster::attach_hull() {
  /// Attempt to attach this device to the hull, return success status
  vessel->devices_hull.push_back(this);
  vessel->thrusters.push_back(this);
  status = statustype::ON_HULL;
  return true;
}

void thruster::remove_hull() {
  /// Remove this device from the hull only (leave it attached to any ship)
  /// Note: not safe to be called in an iteration of hull devices!
  if(!vessel) {
    std::cout << "ERROR: tried to remove hull device " << get_name() << " which is already not attached to a vessel." << std::endl;
    return;
  }
  vessel->devices_hull.remove(this);
  vessel->thrusters.remove(this);
  throttle = 0.0;
  status = statustype::UNMOUNTED;
}

unsigned int thruster::get_port_in_count() {
  /// How many input ports it has
  return 1;
}

std::string thruster::get_port_in_name(unsigned int port) {
  /// Name of the input port numbered n
  switch(port) {
  case 0:
    return "throttle";
  default:
    return "";
  }
}

std::string thruster::get_port_in_description(unsigned int port) {
  /// Description of the input port numbered n
  switch(port) {
  case 0:
    {
      std::stringstream desc;
      desc << "The requested throttle value for this thruster, from 0 to 1."
              "  " << get_port_in_connstatus(port);
      return desc.str();
    }
  default:
    return "";
  }
}

bool thruster::get_port_in_required(unsigned int port __attribute__((__unused__))) {
  /// Whether an input on this port is necessary for this device to operate
  // all ports are required
  return true;
}

unsigned int thruster::get_port_out_count() {
  /// How many output ports it has
  return 6;
}

std::string thruster::get_port_out_name(unsigned int port) {
  /// Name of the output port numbered n
  switch(port) {
  case 0:
    return "throttle value";
  case 1:
    return "thrust";
  case 2:
    return "fuel flow rate";
  case 3:
    return "oxidiser flow rate";
  case 4:
    return "temperature";
  case 5:
    return "pressure";
  default:
    return "";
  }
}

std::string thruster::get_port_out_description(unsigned int port) {
  /// Description of the output port numbered n
  // see https://en.wikipedia.org/wiki/TR-201
  switch(port) {
  case 0:
    return "The effective current throttle setting, from 0 to 1.";
  case 1:
    return "The current thrust, in Newtons.";
  case 2:
    return "The flow rate of the primary fuel, in kg/s.";
  case 3:
    return "The flow rate of the oxidiser, in kg/s.";
  case 4:
    return "The temperature of the combustion chamber, in Kelvin.";
  case 5:
    return "The pressure inside the combustion chamber, in Pa.";
  default:
    return "";
  }
}

double thruster::get_port_out_data(unsigned int port) {
  /// Query the data on the specified out port
  switch(port) {
  case 0:
    return throttle;
  case 1:
    return thrust_magnitude;
  case 2:
    return flowrate_fuel;
  case 3:
    return flowrate_oxi;
  case 4:
    return temperature;
  case 5:
    return pressure;
  default:
    return 0.0;
  }
}

double thruster::get_thrust_max() {
  /// Return the max thrust of this type of engine
  return 50000.0;                 // based roughly on https://en.wikipedia.org/wiki/Descent_Propulsion_System
}

Vector3d thruster::get_thrust() {
  return thrust;
}
double thruster::get_thrust_magnitude() {
  return thrust_magnitude;
}
double thruster::get_flowrate_fuel() {
  return flowrate_fuel;
}
double thruster::get_flowrate_oxi() {
  return flowrate_oxi;
}
double thruster::get_temperature() {
  return temperature;
}
double thruster::get_pressure() {
  return pressure;
}

void thruster::update_thrust() {
  /// Return the 3D vector representing the current thrust (N) of this thruster, relative to the spacecraft
  if(throttle != 0.0) {
    thrust_magnitude = get_thrust_max() * throttle;
    thrust.x = 0.0;
    thrust.y = 0.0;
    thrust.z = thrust_magnitude;
    thrust.rotate(rotation);
  } else {
    thrust_magnitude = 0.0;
    thrust.x = 0.0;
    thrust.y = 0.0;
    thrust.z = 0.0;
  }
}

void thruster::update_flowrate_fuel() {
  /// Fuel flow rate, kg/s
  // based on https://en.wikipedia.org/wiki/TR-201 - Aerozine 50 (N2H4/UDMH)
  flowrate_fuel = 8.92 * throttle;
}

void thruster::update_flowrate_oxi() {
  /// Oxidiser flow rate, kg/s
  // based on https://en.wikipedia.org/wiki/TR-201 - Dinitrogen tetroxide (N2O4)
  flowrate_oxi = 5.62 * throttle;
}

void thruster::update_temperature() {
  /// Combustion chamber temperature (and by association, exhaust temperature), K
  // based on https://en.wikipedia.org/wiki/TR-201, assuming 3369.15K optimal, see https://en.wikipedia.org/wiki/Liquid_rocket_propellants
  // re. effect of throttling on temperature and pressure: http://forum.nasaspaceflight.com/index.php?topic=23620.0
  if(throttle != 0.0) {
    temperature = (3369.15 - 50) + (throttle * 100);   // allow ~100C either side of ideal
  } else {
    //temperature = 33;       // approximate solar system vacuum temperature minimum (~= minimum of surface of Pluto)
    //temperature *= 0.98;    // gradual smooth cooling
    temperature = (temperature * 0.75) + (vessel->get_temperature_hull() * 0.25);    // gradual smooth cooling to hull temp
  }
}

void thruster::update_pressure() {
  /// Combustion chamber pressure, Pa
  // based on https://en.wikipedia.org/wiki/TR-201, see https://en.wikipedia.org/wiki/Liquid_rocket_propellants
  double targetpressure;
  if(throttle != 0.0) {
    targetpressure = 719407.5 * throttle;
  } else {
    targetpressure = 0.0;
  }
  pressure = (pressure * 0.75) + (targetpressure * 0.25);   // gradual smooth pressure changes
}

void thruster::update_throttle(double newthrottle) {
  /// Sanity checked throttle update
  if(newthrottle < 0.0) {
    newthrottle = 0.0;
  } else if(newthrottle > 1.0) {
    newthrottle = 1.0;
  }
  throttle = newthrottle;
  //std::cout << "DEBUG: " << get_name() << " throttle is now " << throttle * 100 << "%" << std::endl;
}

void thruster::update() {
  /// Update the output states and respond to changes in input
  // update the throttle and all consequent values
  if(ports_in[0].target) {
    update_throttle(ports_in[0].target->get_port_out_data(ports_in[0].target_port));
    update_thrust();
    update_flowrate_fuel();
    update_flowrate_oxi();
    // throttle stays open if disconnected unless specifically closed
  }
  // the following values can alter regardless of throttle setting
  update_temperature();
  update_pressure();
}

void thruster::update_if_time() {
  /// Run the update function only if it's time for an update, and reset the update clock
  boost::chrono::time_point<boost::chrono::high_resolution_clock, boost::chrono::duration<double>> const time_now(boost::chrono::high_resolution_clock::now());
  if(time_now >= time_nextupdate) {
    update();
    time_nextupdate = time_now + boost::chrono::duration<double>(boost::chrono::milliseconds(250));  // 4 updates per second
  }
}
