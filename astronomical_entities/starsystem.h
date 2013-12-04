#ifndef STARSYSTEM_H_INCLUDED
#define STARSYSTEM_H_INCLUDED

#include <string>
#include <list>
#include "vmath.h"
#include "random_engine.h"

class body;
class star;

class starsystem : public random_engine {
  /// A star system
private:
  std::string name;
  std::string designation;
  std::string description;

public:
  virtual std::string get_name();
  virtual void set_name(std::string newname);
  virtual std::string get_designation();
  virtual void set_designation(std::string newdesignation);
  virtual std::string get_description();
  virtual void set_description(std::string newdescription);

  std::list<body*> bodies;            // list of stellar and planetary bodies
  star *primary;                      // the primary body of this system

  Vector3d position;                  // relative to the galaxy
  Vector3d velocity;                  // relative to the galaxy

  starsystem();
  ~starsystem();
};

#endif // STARSYSTEM_H_INCLUDED
