#include "body.h"
#include <iostream>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>

body::body()
  : mass(0.0),
    radius(0.0),
    gm(0.0) {
  /// Default constructor
  //rotation = spin = Quatd::fromAxisRot(Vector3d(0.0, 1.0, 0.0), 0.0);           // null rotation quaternion
  rotation = spin = Quatd(1.0, 0.0, 0.0, 0.0);                                  // null rotation quaternion
}

body::~body() {
  /// Default destructor
}

std::string body::get_name() {
  if(!name.empty()) {
    return name;
  } else {
    // random asteroid type name, as in http://en.wikipedia.org/wiki/List_of_minor_planets:_1%E2%80%931000
    random_reset();                                                             // reset the generator to its seed
    std::stringstream randomname;
    randomname << get_random_uint(10, 118161) << " " << get_random_name_ancient();
    return randomname.str();
  }
}

void body::set_name(std::string const &newname) {
  name = newname;
}

std::string body::get_type() {
  /// Return the type of object this is
  return "astronomical body";
}

std::string body::get_designation() {
  if(designation.size() != 0) {
    return designation;
  } else {
    // generate a random astronomical designation in the standard style "2003 VB12"
    // see http://en.wikipedia.org/wiki/Provisional_designation_in_astronomy
    random_reset();                                                             // reset the generator to its seed
    std::stringstream designation;
    designation << get_random_uint(1926, 2059)                                  // year
                << " "
                << get_random_char_alpha_upper()                                // month (allow Z since we're lazy)
                << get_random_char_alpha_upper()                                // order of discovery letter
                << get_random_uint(1, 530);                                     // order of discovery number
    return designation.str();
  }
}

void body::set_designation(std::string const &newdesignation) {
  designation = newdesignation;
}

std::string body::get_description() {
  return description;
}

void body::set_description(std::string const &newdescription) {
  description = newdescription;
}

double body::get_mass() {
  if(mass != 0.0) {
    return mass;
  } else {
    // no mass assigned, take a potshot at one - in the small 100m planetesimal / asteroid scale
    // 100m asteroid weighs about 657640030kg?
    random_reset();
    return get_random_double(300000000.0, 1000000000.0);                        // averaging 650 tons +- 50%
  }
}

void body::set_mass(double newmass) {
  mass = newmass;
  update_gm();
}

double body::get_radius() {
  if(radius != 0.0) {
    return radius;
  } else {
    // make up a radius for this based on what's likely for the mass
    // i.e. 253 Mathilde = 10330000000000000kg (1.033 * 10^17), radius ~= 25000m
    // assuming volume as sphere ~= 4/3 PI * r^3 ~= 65449846949787m^3
    // asteroid density = mass / volume ~= 157.83
    // inverse = 0.00633
    double const volume = get_mass() * 0.00633;
    return pow(volume / ((4.0 / 3.0) * M_PI), 1.0 / 3.0);                       // radius from volume of sphere
  }
}

void body::set_radius(double newradius) {
  radius = newradius;
  update_model();
}

double body::get_kinetic_energy() {
  /// Return the kinetic energy of this body in Joules
  return 0.5 * get_mass() * velocity.length() * velocity.length();
}

void body::update_state(double time __attribute__((__unused__)), double deltatime) {
  /// Re-calculate current velocity and position based on orbital data
  // by default bodies just maintain momentum
  //std::cout << "INFO: Called default update_state on " << name << std::endl;
  position += velocity * deltatime;
  rotation = rotation * (spin * deltatime);
}

void body::update_gm() {
  /// Re-cache the standard gravitational parameter
  gm = (gravitational_constant * get_mass());
}

void body::update_model() {
  /// Make any necessary updates to the display model
  // nothing to be done by default
}

bool body::check_within_physical_influence(Vector3d const &absolute_coords) {
  /// Check if we're within range for physical interaction (collision etc)
  return check_within_physical_influence_rel(absolute_coords - position);
}

bool body::check_within_physical_influence_rel(Vector3d const &relative_coords) {
  /// Check if we're within range for physical interaction (collision etc) - local version
  // bounding box version
  double const thisradius = get_radius();
  if(abs(relative_coords.x) < thisradius &&
     abs(relative_coords.y) < thisradius &&
     abs(relative_coords.z) < thisradius) {
    return true;
  } else {
    return false;
  }
}

bool body::check_within_physical_influence_rel(double thisradius) {
  /// Check if we're within range for physical interaction (collision etc) - local distance
  if(thisradius <= get_radius()) {
    // assume a bounding sphere
    return true;
  } else {
    return false;
  }
}

Vector3d body::get_collision(Vector3d const &absolute_coords) {
  /// Check for collision relative to the same reference sphere as the object and return the vector of a surface normal or Vector(0, 0, 0) otherwise
  return get_collision_rel(position - absolute_coords);
}

Vector3d body::get_collision_rel(Vector3d const &relative_coords) {
  /// Check for collision relative to the centre and return the vector of a surface normal or Vector(0, 0, 0) otherwise
  // this simply checks for a smooth spherical collision
  double distance = relative_coords.length();
  if(distance <= get_radius()) {
    return relative_coords.normalise_copy();
  } else {
    return Vector3d(0.0, 0.0, 0.0);
  }
}

double body::get_gravity_accel(Vector3d const &coords) {
  /// Return the acceleration due to gravity at a point in the same reference sphere as this object
  return get_gravity_accel_rel(Vector3d(position - coords).length());
}

double body::get_gravity_accel_rel(Vector3d const &relative_coords) {
  /// Return the acceleration due to gravity at these coords relative to the centre
  return get_gravity_accel_rel(relative_coords.length());
}

double body::get_gravity_accel_rel(double relative_distance) {
  /// Return the acceleration due to gravity at this distance from the centre
  //std::cout << "            DEBUG: get_gravity_accel_rel before called with relative_distance=" << relative_distance << std::endl;
  // Newtonian:
  // g = (G * m1) / (r^2)
  //return (gravitational_constant * get_mass()) / pow(relative_distance, 2);
  // optimised form:
  return (gravitational_constant * get_mass()) / (relative_distance * relative_distance);
}

Vector3d body::get_gravity_accel_v3(Vector3d const &coords) {
  /// Return the acceleration due to gravity at a point in the same reference sphere as this object - vector version
  //std::cout << "        DEBUG: get_gravity_accel_v3 before called with coords=" << coords << ", position=" << position << std::endl;
  return get_gravity_accel_rel_v3(position - coords);
}

Vector3d body::get_gravity_accel_rel_v3(Vector3d const &relative_coords) {
  /// Return the acceleration due to gravity at these coords relative to the centre - vector version
  //std::cout << "          DEBUG: get_gravity_accel_rel_v3 before called with relative_coords=" << relative_coords << std::endl;
  return relative_coords.normalise_copy() * get_gravity_accel_rel(relative_coords.length());
}

double body::get_gravity_accel_surface() {
  /// Wrapper to return acceleration due to gravity at presumed surface
  return get_gravity_accel_rel(get_radius());
}

double body::get_gravity_accel(Vector3d const &coords, Vector3d const &thisvelocity) {
  /// Return the acceleration due to gravity at a point in the same reference sphere as this object - relativistic version
  return get_gravity_accel_rel(position - coords, thisvelocity - velocity);
}

double body::get_gravity_accel_rel(Vector3d const &relative_coords, Vector3d const &thisvelocity) {
  /// Return the acceleration due to gravity at these coords relative to the centre - relativistic version
  return get_gravity_accel_rel(relative_coords.length(), thisvelocity - velocity);
}

double body::get_gravity_accel_rel(double relative_distance, Vector3d const &thisvelocity) {
  /// Return the acceleration due to gravity at this distance from the centre - relativistic version
  // Newtonian:
  // g = (G * m1) / (r^2)
  double dist_sq = relative_distance * relative_distance;
  double const newtonian = -((gravitational_constant * get_mass()) / dist_sq);
  // Schwarzschild solution:
  // g = (G * m1) / (r^2) + ((3 * G * m1 * (v^2)) / ((r^2) * (c^2)))
  double const relativistic = -((3 * gravitational_constant * get_mass() * (thisvelocity - velocity).lengthSq()) / (dist_sq * (speed_of_light * speed_of_light)));
  //std::cout << "DEBUG: Newtonian component:    " << newtonian    << std::endl;
  //std::cout << "DEBUG: Relativistic component: " << relativistic << std::endl;
  return newtonian + relativistic;
}

Vector3d body::get_gravity_accel_v3(Vector3d const &coords, Vector3d const &thisvelocity) {
  /// Return the acceleration due to gravity at a point in the same reference sphere as this object - relativistic vector version
  return get_gravity_accel_rel_v3(position - coords, thisvelocity - velocity);
}

Vector3d body::get_gravity_accel_rel_v3(Vector3d const &relative_coords, Vector3d const &thisvelocity) {
  /// Return the acceleration due to gravity at these coords relative to the centre - relativistic vector version
  return relative_coords.normalise_copy() * get_gravity_accel_rel(relative_coords.length(), thisvelocity - velocity);
}

double body::get_escape_vel(Vector3d const &coords) {
  /// Return the required escape velocity at a point in the same reference sphere as this object
  return get_escape_vel_rel(Vector3d(position - coords).length());
}

double body::get_escape_vel_rel(Vector3d const &relative_coords) {
  /// Return the required escape velocity at these coords relative to the centre
  return get_escape_vel_rel(relative_coords.length());
}

double body::get_escape_vel_rel(double relative_distance) {
  /// Return the required escape velocity at this distance from the centre
  // v_e = sqrt((2 * G * m) / r)
  return sqrt((2 * gravitational_constant * get_mass()) / relative_distance);
}

void body::render_diagram(double scale, bool labels) {
  /// Render in the orthographic diagram view
  glPushMatrix();
  // move into position
  glTranslated(position.x, position.y, position.z);
  // undo rotation - billboard effect
  Matrix4d modelview;
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  for(unsigned int i = 0; i != 3; ++i) {
    for(unsigned int j = 0; j != 3; ++j) {
      if(i == j) {
        modelview[i * 4 + j] = 1.0;
      } else {
        modelview[i * 4 + j] = 0.0;
      }
    }
  }
  // set the modelview matrix with no rotations and scaling
  glLoadMatrixd(modelview);

  double thisradius = get_radius();
  if(thisradius * scale < 1) {
    thisradius = 1 / scale;
  }

  // circle outline
  glColor4dv(Vector4d(1.0, 1.0, 1.0, 1.0));
  double const circlestep = M_PI / 4.0;
  glBegin(GL_LINE_LOOP);
  for(double angle = 0.0; angle <= M_PI * 2.0; angle += circlestep) {
    glVertex3d(sin(angle) * thisradius, cos(angle) * thisradius, 0.0);
  }
  glEnd();

  glPopMatrix();                                                                // restore position & rotation
}

void body::render_visible(unsigned int depth) {
  /// Render in the visible spectrum
  // TODO
}

void body::render_radio() {
  /// Render in the radio spectrum, i.e. radar reflection
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void body::render_infrared() {
  /// Render in the infrared spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

void body::render_ultraviolet() {
  /// Render in the ultraviolet spectrum
  std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
}

