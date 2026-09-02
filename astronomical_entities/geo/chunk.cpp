#include "chunk.h"
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "corner.h"

chunk::chunk()
  : corners{nullptr, nullptr, nullptr} {
  /// Default constructor
}

chunk::~chunk() {
  /// Default destructor
}

void chunk::set_corners(corner *corner1, corner *corner2, corner *corner3) {
  /// simple function to set three corner pointers at once
  corners[0] = corner1;
  corners[1] = corner2;
  corners[2] = corner3;
  //update();
}

void chunk::update() {
  vector3d const line1 = corners[1]->coords - corners[0]->coords;
  vector3d const line2 = corners[2]->coords - corners[0]->coords;
  normal = line1.cross(line2);
  normal.normalise();
}

void chunk::render() {
  /// Draw this chunk and all its child chunks if appropriate
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_SPECULAR,            vector4f(0.8, 0.8, 0.8, 1.0));
  glMaterialfv(GL_FRONT, GL_EMISSION,            vector4f(0.0, 0.0, 0.0, 1.0));
  glMaterialf( GL_FRONT, GL_SHININESS,           20.0);                         // 0 to 127

  glBegin(GL_TRIANGLES);
  glNormal3dv(normal);
  //glNormal3dv(corners[0]->coords.normalise_copy());
  glVertex3dv(corners[0]->coords);
  //glNormal3dv(corners[1]->coords.normalise_copy());
  glVertex3dv(corners[1]->coords);
  //glNormal3dv(corners[2]->coords.normalise_copy());
  glVertex3dv(corners[2]->coords);
  glEnd();
}
