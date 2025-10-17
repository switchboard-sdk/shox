# SHOX: Synervoz Header Only Classes
Open source header-only utility and helper classes by Synervoz.

---
## Installation

### CMake

You can get SHOX via CMake's `FetchContent` feature:

```cmake
include(FetchContent)
FetchContent_Declare(
        shox
        GIT_REPOSITORY https://github.com/switchboard-sdk/shox
        GIT_TAG        606d4c1994cd78dc7d0d81bd88dbae66802a9810 # Adjust accordingly
)
FetchContent_MakeAvailable(shox)
```
Then link it to your target:
```cmake
target_link_libraries(your_target PRIVATE shox::shox)
```

Alternatively, you could provide a `Findshox.cmake` module that contains the `FetchContent` calls above. Then in your CMakeLists.txt:

```cmake
# Make sure Findshox.cmake is in your CMAKE_MODULE_PATH 
# either by having it in the same directory as CMakeLists.txt 
# or appending to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

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