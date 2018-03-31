
project(Anthill CXX)

set(CURL_MIN_VERSION "7.28.0")

file(GLOB SOURCES
    src/anthill/*.cpp
    src/anthill/requests/*.cpp
    src/anthill/services/*.cpp
    src/anthill/states/*.cpp
)

include_directories(
	include
)

set(CURL_STATICLIB ON CACHE BOOL "Curl should be build statically")
set(BUILD_SHARED_LIBS OFF)

set(LIBUV_LIBRARY uv CACHE STRING "uv library name")
set(CURLCPP_LIBRARY curlcpp CACHE STRING "curlcpp library name")
set(JSONCPP_LIBRARY jsoncpp_lib_static CACHE STRING "jsoncpp library name")
set(UWS_LIBRARY uWS CACHE STRING "uWebSockets library name")
set(OPENSSL_LIBRARY openssl CACHE STRING "OpenSSL library name")
set(ANTHILL_LIBRARY AnthillRuntime CACHE STRING "AnthillRuntime library name")

# The library itself
add_library(AnthillRuntime STATIC ${SOURCES})

if( APPLE )
	set( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
else()
	target_compile_features(AnthillRuntime PRIVATE cxx_range_for)
endif()

if (UNIX AND NOT APPLE)
	target_link_libraries(AnthillRuntime curl)
else ()
	target_link_libraries(AnthillRuntime libcurl)
endif ()

target_include_directories(AnthillRuntime PUBLIC 
	${OPENSSL_SOURCE_DIR}
	${JSONCPP_INCLUDE_DIR}
	${CURLCPP_INCLUDE_DIR}
	"${UWS_DIR}/src"
)

target_link_libraries(AnthillRuntime ${CURLCPP_LIBRARY} ${JSONCPP_LIBRARY} ${UWS_LIBRARY} ${OPENSSL_LIBRARY})

if (APPLE)
	add_definitions(-DUSE_DARWINSSL)

	target_link_libraries(AnthillRuntime "-framework CoreFoundation" )
	target_link_libraries(AnthillRuntime "-framework Security" )
endif (APPLE)

