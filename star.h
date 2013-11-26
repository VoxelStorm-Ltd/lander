#ifndef STAR_H_INCLUDED
#define STAR_H_INCLUDED

#include "body.h"

class star : public body {
  /// A star or stellar object large enough to be undergoing fusion
public:
  double brightness;            // absolute brightness locally

  star();
  ~star();

  double get_magnitude();
};

#endif // STAR_H_INCLUDED
