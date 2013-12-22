#ifndef DUSTCLOUD_H_INCLUDED
#define DUSTCLOUD_H_INCLUDED

#include <string>
#include "body.h"

class dustcloud : public body {
public:
  dustcloud();
  ~dustcloud();

  std::string get_name();
  std::string get_designation();
  std::string get_description();
  double get_radius();
  //asdfdouble get_kinetic_energy();

  //bool check_within_physical_influence_rel(double thisradius);

};

#endif // DUSTCLOUD_H_INCLUDED
