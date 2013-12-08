#ifndef INSTRUMENTPANEL_H_INCLUDED
#define INSTRUMENTPANEL_H_INCLUDED

#include <list>
#include "vmath.h"

class device;
class spacecraft;

class instrumentpanel {
  /// A physical surface that instruments can be attached to
public:
  std::list<device*> devices;             // devices attached to this panel

  Vector3d position;                      // its location relative to cockpit origin, in metres
  Vector3d size;                          // how much physical space it takes up, in metres
  Quatd rotation;                         // its rotation relative to the cockpit

  spacecraft *vessel;                     // what ship it's attached to

  instrumentpanel();
  ~instrumentpanel();

  void attach(spacecraft *to_vessel);
  void remove();
  void destroy();
  void render();
};

#endif // INSTRUMENTPANEL_H_INCLUDED
