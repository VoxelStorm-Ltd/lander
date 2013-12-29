#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

#include <vector>

class corner;
class chunk;

class region {
  /** Region triangles are subdivided into chunk triangles
   **     _ Z
   **     /|       Horizontal chunk coordinates are in pairs, following the lower
   **    /\        and left axes, at a 60 degree angle to each other.  Extracting
   **   /\/\       Cartesian coordinates requires conversion from this scheme.
   **  /\/\/\
   ** *-------> X
   **/
public:
  corner *parent_corners[3];

  std::vector<std::vector<corner>> corners;
  std::vector<std::vector<chunk>>  chunks;

  region();
  ~region();

  void set_corners(corner *corner1, corner *corner2, corner *corner3);

  void render();
};

#endif // REGION_H_INCLUDED
