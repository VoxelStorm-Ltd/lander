#ifndef INSTRUMENTPANEL_H_INCLUDED
#define INSTRUMENTPANEL_H_INCLUDED

#include <list>
#include "vectorstorm/vectorstorm.h"

class device;
class spacecraft;

class instrumentpanel {
  /// A physical surface that instruments can be attached to
public:
  std::list<device*> devices;                                                   // devices attached to this panel

  Vector3d position;                                                            // its location relative to cabin origin, in metres
  Vector3d size;                                                                // how much physical space it takes up, in metres
  Quatd rotation;                                                               // its rotation relative to the cabin

  spacecraft *vessel;                                                           // what ship it's attached to

  bool pickpoint_show;                                                          // whether to show cursor
  Vector2d pickpoint;                                                           // cursor
  device *pickeddevice;                                                         // what device we've selected

  instrumentpanel();
  ~instrumentpanel();

  void attach(spacecraft *to_vessel);
  void remove();
  void destroy();
  void render();
  device *pick(Vector3d const &origin, Vector3d const &pickvector);
};

#endif // INSTRUMENTPANEL_H_INCLUDED
