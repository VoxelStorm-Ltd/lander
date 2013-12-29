#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <vector>

class corner;

class chunk {
public:
  corner *corners[3];

  std::vector<corner> heightmap;

  chunk();
  ~chunk();
};

#endif // CHUNK_H_INCLUDED
