set(test_sources
    Datatypes.cpp
    Dht.cpp
)

include(cpp11)

add_executable(testi2p ${test_sources})

# Botan
include_directories(BEFORE testi2p ${BOTAN_INCLUDE_DIRS})
target_link_libraries(testi2p ${BOTAN_LIBRARIES})

# zlib
include_directories(BEFORE testi2p ${ZLIB_INCLUDE_DIRS})
target_link_libraries(testi2p ${ZLIB_LIBRARIES})

# Boost
include_directories(BEFORE testi2p ${Boost_INCLUDE_DIRS})
target_link_libraries(testi2p ${Boost_LIBRARIES})
add_definitions(-DBOOST_ALL_DYN_LINK)

# i2pcpp
include_directories(BEFORE testi2p ${CMAKE_SOURCE_DIR})
include_directories(BEFORE testi2p ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(testi2p datatypes util i2p)
