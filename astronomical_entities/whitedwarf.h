#ifndef WHITEDWARF_H_INCLUDED
#define WHITEDWARF_H_INCLUDED

#include "star.h"

class whitedwarf : public star {
  /// White dwarf stars
public:
  whitedwarf();
  ~whitedwarf();

  std::string get_type();
};

#endif // WHITEDWARF_H_INCLUDED
