#ifndef _SERVERDATASTRUCT_
#define _SERVERDATASTRUCT_

#include <cstddef>
#include <cstdint>
#include <vector>

struct ServerDataStruct {
  std::vector<char> buffer;
  std::size_t length;
  std::uint32_t padding;
};

#endif