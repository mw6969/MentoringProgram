#ifndef _CLIENTDATASTRUCT_
#define _CLIENTDATASTRUCT_

#include <ios>
#include <vector>

struct ClientDataStruct {
  std::vector<char> buffer;
  std::streamsize length;
  std::streamsize size;
};

#endif