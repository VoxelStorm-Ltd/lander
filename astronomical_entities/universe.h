#ifndef UNIVERSE_H_INCLUDED
#define UNIVERSE_H_INCLUDED

#include <forward_list>
#include <vector>
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
};

#endif // UNIVERSE_H_INCLUDED
