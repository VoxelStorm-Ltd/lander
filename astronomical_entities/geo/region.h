#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

#include <vector>
#include "vmath.h"

class corner;

class region {
  /// Region triangles are subdivided into triangular quadtrees
  /// See https://docs.google.com/drawings/d/1ud3-i8Ua4CfPrwwuEnJJ4xYh_KOTPT-oA6x1aeoe1o0/edit
public:
  corner *parent_corners[3];
  Vector3d normal;

  corner *inner_corners[3];
  region *regions[4];

  region();
  ~region();

  void set_corners(corner *corner1, corner *corner2, corner *corner3);
  void subdivide(unsigned int depth = 0);
  void update();
  void render_visible(unsigned int depth = 0);
};

#endif // REGION_H_INCLUDED
