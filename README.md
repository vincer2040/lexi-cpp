# lexi-cpp

a client for [lexidb](https://github.com/vincer2040/lexidb), an in memory data structure data base 

## Installation

### Requirements

1. cmake >= 3.11
2. C++ 17

### Install

1. Clone this repo

```console
$ git clone git@github.com:vincer2040/lexi-cpp.git
```

2. make a build directory

```console
$ mkdir build && cd build
```

3. build it 

from within the build directory: 

```console
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

4. Run tests (optional)

from within the build directory:

```console
$ make test
```

5. install

from within the build directory: 

```console
$ sudo make install
```

The default install path is /usr/local/lib

## usage 

```cpp
#include "lexi.hh"
#include <assert.h>
#include <iostream>

int main(void) {
  Lexi client("127.0.0.1", <port>);
  client.connect();
  LexiType set_result = client.set("key", "value");
  assert(set_result.type, LexiTypeT::Simple);
  auto res = std::get<std::shared_ptr<std::string>>(set_result.data);
  std::cout << *res << "\n";  // "OK"

  LexiType get_result = client.get("key");
  assert(get_result.type, LexiTypeT::Bulk);
  auto value = std::get<std::shared_ptr<std::string>>(get_result.data);
  std::cout << *value << "\n"; // "value"

  LexiType del_result = client.del("key");
  assert(del_result.type, LexiTypeT::Simple);
  auto deleted = std::get<std::shared_ptr<std::string>>(del_result.data);
  std::cout << *deleted << "\n";  // "OK"
  
  return 0;
}
```

For a more complete example, see examples/main.cc

