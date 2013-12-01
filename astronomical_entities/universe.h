#ifndef UNIVERSE_H_INCLUDED
#define UNIVERSE_H_INCLUDED

#include <forward_list>
#include <vector>
#include "vmath.h"

class astronaut;
class starsystem;
class star;
class physicsbody;

class universe {
  /// The global container for everything that exists
public:
  double time;                                            // current time
  std::forward_list<astronaut*> astronauts;               // list of astronauts
  std::vector<starsystem*> starsystems;                   // list of stars & star systems
  //std::forward_list<physicsbody*> physicsbodies;          // list of physics bodies to simulate every tick

  starsystem *currentsystem;

  universe();
  ~universe();

  void render();

  void make_explosion(Vector3d coords, double energy);
  void destroy_range( Vector3d coords, double range);
};

#endif // UNIVERSE_H_INCLUDED
