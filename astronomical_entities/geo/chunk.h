#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <vector>
#include "vmath.h"

class corner;

class chunk {
public:
  corner *corners[3];

  Vector3d normal;

  std::vector<corner> heightmap;

  chunk();
  ~chunk();

  void set_corners(corner *corner1, corner *corner2, corner *corner3);
  void update();
  void render();
};

#endif // CHUNK_H_INCLUDED
