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
#include <iostream>

int main(void) {
  lexi::client client("<address>", "<port>");
  client.connect();
  client.authenticate("<username>", "<password>");

  lexi::lexi_data set = client.set("key", "value");
  std::cout << set << '\n'; // ok

  lexi::lexi_data get = client.get("key");
  std::cout << get << '\n'; // "value"

  lexi::lexi_data del = client.del("key");
  std::cout << del << '\n'; // ok

  lexi::lexi_data set_int = client.set("key", 1337);
  std::cout << set_int << '\n'; // ok

  lexi::lexi_data get_int = client.get("key");
  std::cout << get_int << '\n'; // (int) 1337
  return 0;
}
```

For a more complete example, see examples/main.cc


