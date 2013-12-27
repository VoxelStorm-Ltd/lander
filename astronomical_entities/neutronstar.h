#ifndef NEUTRONSTAR_H_INCLUDED
#define NEUTRONSTAR_H_INCLUDED

#include "star.h"

class neutronstar : public star {
public:
  neutronstar();
  virtual ~neutronstar();

  virtual std::string get_type();
  virtual std::string get_description();
};

#endif // NEUTRONSTAR_H_INCLUDED
