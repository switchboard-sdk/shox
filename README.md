# SHOX: Synervoz Header Only Classes
Open source header-only utility and helper classes by Synervoz.

---
## Installation

### CMake

```cmake
find_package(shox REQUIRED)
target_link_libraries(your_target PRIVATE shox::shox)
```

### Package

Fetch the tarball from the releases.

## Usage

Include the header files you need in your project:

```cpp
#include <shox/your_header_file.h>

// using namespace shox;

shox::RingBuffer<float> buffer(1024);
```