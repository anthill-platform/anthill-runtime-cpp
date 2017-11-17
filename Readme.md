# C++ Runtime for Anthill Platform

This Runtime allows you to have all of the 
<a href="https://github.com/anthill-services/anthill">Anthill Platform</a> 
functionality inside of your C++ based game.

# Installation

This library uses CMake, so just clone this repository to your third-party libraries, and do

```
add_subdirectory(../third-party/anthill-runtime-cpp)
...
include_directories(../third-party/anthill-runtime-cpp/include)
```

# Dependencies

You would need to install all dependencies as well:

1. [curl](https://github.com/curl/curl) (also may require `openssl` and `zlib`)
2. [curlcpp](https://github.com/JosephP91/curlcpp)
3. [uWebSockets](https://github.com/uNetworking/uWebSockets)
4. [json-cpp](https://github.com/open-source-parsers/jsoncpp.git)

Please feel free to contribute to the CMake configuration.