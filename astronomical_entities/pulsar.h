#ifndef PULSAR_H_INCLUDED
#define PULSAR_H_INCLUDED

#include "neutronstar.h"

class pulsar : public neutronstar {
public:
  double period;                        // the rotational period of this pulsar

  pulsar();
  ~pulsar();

  virtual std::string get_designation();
  virtual std::string get_description();
};

#endif // PULSAR_H_INCLUDED
