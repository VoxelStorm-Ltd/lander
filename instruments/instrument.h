#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

class instrument {
public:
  instrument();
  ~instrument();

  virtual void update();
  virtual void render();
};

#endif // INSTRUMENT_H_INCLUDED
