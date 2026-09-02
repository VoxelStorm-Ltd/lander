#ifndef BODY_H_INCLUDED
#define BODY_H_INCLUDED

#include <string>
#include "vectorstorm/vectorstorm.h"
#include "random_engine.h"
#include "menu.h"

class body : public random_engine, public menu {
  /// An astronomical body of some sort, following basic newtonian mechanics
protected:
  body();                                                                       // prevent this class being instantiated directly

  std::string name;
  std::string designation;
  std::string description;

  double mass;                                                                  // in kg
  double radius;                                                                // visual radius

public:
  double static constexpr gravitational_constant = 0.0000000000667;             // G = 6.67 * 10^-11 N(m/kg)^2
  double static constexpr speed_of_light = 299792458;                           // c = 299,792,458 m/s
  double static constexpr gas_constant = 8.3144621;                             // R = 8.3144621(75) J/(mol K)
  double gm;                                                                    // GM - standard gravitational parameter, cached

  vector3d position;                                                            // in m, relative to its star system
  vector3d velocity;                                                            // in m, relative to its star system
  quatd rotation;                                                               // absolute
  quatd spin;                                                                   // per second, absolute

  virtual ~body();

  // getters and setters
  virtual std::string get_name();
  virtual void set_name(std::string const &newname);
  virtual std::string get_type();
  virtual std::string get_designation();
  virtual void set_designation(std::string const &newdesignation);
  virtual std::string get_description();
  virtual void set_description(std::string const &newdescription);
  virtual double get_mass();
  virtual void set_mass(double newmass);
  virtual double get_radius();
  virtual void set_radius(double newradius);
  virtual double get_kinetic_energy();
  virtual bool check_within_physical_influence(vector3d const &absolute_coords);
  virtual bool check_within_physical_influence_rel(vector3d const &relative_coords);
  virtual bool check_within_physical_influence_rel(double thisradius);

  // update
  virtual void update_state(double time, double deltatime);
  void update_gm();
  virtual void update_model();

  // render
  virtual void render_diagram(double scale, bool labels = false);
  virtual void render_visible(unsigned int depth = 0);
  virtual void render_radio();
  virtual void render_infrared();
  virtual void render_ultraviolet();

  // collisions
  virtual vector3d get_collision(    vector3d const &absolute_coords);
  virtual vector3d get_collision_rel(vector3d const &relative_coords);
  // acceleration due to gravity
  double get_gravity_accel(          vector3d const &absolute_coords);
  double get_gravity_accel_rel(      vector3d const &relative_coords);
  double get_gravity_accel_rel(      double relative_distance);
  vector3d get_gravity_accel_v3(     vector3d const &absolute_coords);
  vector3d get_gravity_accel_rel_v3( vector3d const &relative_coords);
  double get_gravity_accel_surface();
  // relativistic versions:
  double get_gravity_accel(          vector3d const &absolute_coords, vector3d const &thisvelocity);
  double get_gravity_accel_rel(      vector3d const &relative_coords, vector3d const &thisvelocity);
  double get_gravity_accel_rel(      double relative_distance,        vector3d const &thisvelocity);
  vector3d get_gravity_accel_v3(     vector3d const &absolute_coords, vector3d const &thisvelocity);
  vector3d get_gravity_accel_rel_v3( vector3d const &relative_coords, vector3d const &thisvelocity);
  // escape velocity
  double get_escape_vel(             vector3d const &absolute_coords);
  double get_escape_vel_rel(         vector3d const &relative_coords);
  double get_escape_vel_rel(         double relative_distance);
};

#endif // BODY_H_INCLUDED
