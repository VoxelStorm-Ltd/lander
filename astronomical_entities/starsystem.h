#ifndef STARSYSTEM_H_INCLUDED
#define STARSYSTEM_H_INCLUDED

#include <string>
#include <list>
#include "vectorstorm/vectorstorm.h"
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
  std::list<body*> bodies;                                                      // list of stellar and planetary bodies
  star *primary;                                                                // the primary body of this system

  Vector3d position;                                                            // relative to the galaxy
  Vector3d velocity;                                                            // relative to the galaxy

  starsystem();
  ~starsystem();

  std::string get_name();
  void set_name(std::string const &newname);
  std::string get_type();
  std::string get_designation();
  void set_designation(std::string const &newdesignation);
  std::string get_description();
  void set_description(std::string const &newdescription);

  void render_visible();
};

#endif // STARSYSTEM_H_INCLUDED
