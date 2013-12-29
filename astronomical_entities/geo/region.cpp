#include "region.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "corner.h"
#include "chunk.h"

region::region()
  : parent_corners{nullptr, nullptr, nullptr} {
  /// Default constructor
}

region::~region() {
  /// Default destructor
}

void region::set_corners(corner *corner1, corner *corner2, corner *corner3) {
  /// simple function to set three corner pointers at once
  parent_corners[0] = corner1;
  parent_corners[1] = corner2;
  parent_corners[2] = corner3;
}

void region::render() {
  /// Draw this region and all its chunks if appropriate
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            Vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            Vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf( GL_FRONT, GL_SHININESS,           20.0);                           // 0 to 127

  glBegin(GL_TRIANGLES);
  glNormal3dv(parent_corners[0]->coords.normalise_copy());
  glVertex3dv(parent_corners[0]->coords);
  glNormal3dv(parent_corners[1]->coords.normalise_copy());
  glVertex3dv(parent_corners[1]->coords);
  glNormal3dv(parent_corners[2]->coords.normalise_copy());
  glVertex3dv(parent_corners[2]->coords);
  glEnd();
}
