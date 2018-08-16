################################################################################
# clean build, run
# rm -rf CMakeCache.txt, CMakeFiles/, MyCpp11Test; cmake .; make; MyCpp11Test;



################################################################################
# env setting
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)


################################################################################
# gtest
INCLUDE_DIRECTORIES( ../thirdparty/gtest/include )
ADD_LIBRARY( gtest STATIC IMPORTED )
SET_TARGET_PROPERTIES( gtest PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../thirdparty/gtest/bin/Linux/libgtest.a )
MESSAGE("GOOGLETEST LIB PATH : ${CMAKE_SOURCE_DIR}/thirdparty/gtest/bin/Linux/libgtest.a")



# ################################################################################
# # stellite
# INCLUDE_DIRECTORIES( stellite/include )
# ADD_LIBRARY( stellite SHARED IMPORTED )
# SET_TARGET_PROPERTIES( stellite PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/stellite/bin/libstellite_http_client.so )
# LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/stellite/bin/ )
# MESSAGE("STELLITE LIB PATH : ${CMAKE_SOURCE_DIR}/stellite/bin/libstellite_http_client.so")



################################################################################
# main build
ADD_EXECUTABLE( MyCpp11Test 
                test_sample.cpp
                testf_sample.cpp
                main.cpp
                temp.cpp)

TARGET_LINK_LIBRARIES(  MyCpp11Test 
                        gtest
                        pthread)



################################################################################
# install dependencies
# ADD_CUSTOM_COMMAND (
#     TARGET MyStelliteLinuxTest
#     COMMENT "copy so files ..."
#     COMMAND sudo cp ${CMAKE_SOURCE_DIR}/stellite/bin/libstellite_http_client.so /usr/local/lib
#     COMMENT "ldconfig ..."
#     COMMAND sudo ldconfig
# )